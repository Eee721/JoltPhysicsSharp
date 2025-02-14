// Copyright ?Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using static JoltPhysicsSharp.JoltApi;

namespace JoltPhysicsSharp;

public abstract class ObjectVsBroadPhaseLayerFilter : NativeObject
{
    private static readonly Dictionary<IntPtr, ObjectVsBroadPhaseLayerFilter> s_listeners = new();
    private static JPH_ObjectVsBroadPhaseLayerFilter_Procs s_ObjectVsBroadPhaseLayerFilter_Procs;

    public delegate uint ShouldCollideDelegate(IntPtr a, ObjectLayer b, BroadPhaseLayer c);
    static unsafe ObjectVsBroadPhaseLayerFilter()
    {
        var callbackDelegate = new ShouldCollideDelegate(ShouldCollideCallback);
        var callbackPointer = Marshal.GetFunctionPointerForDelegate(callbackDelegate);
        s_ObjectVsBroadPhaseLayerFilter_Procs = new JPH_ObjectVsBroadPhaseLayerFilter_Procs
        {
            ShouldCollide = callbackPointer
        };
        JPH_ObjectVsBroadPhaseLayerFilter_SetProcs(s_ObjectVsBroadPhaseLayerFilter_Procs);
    }

    public ObjectVsBroadPhaseLayerFilter()
        : base(JPH_ObjectVsBroadPhaseLayerFilter_Create())
    {
        s_listeners.Add(Handle, this);
    }

    /// <summary>
    /// Finalizes an instance of the <see cref="ObjectVsBroadPhaseLayerFilter" /> class.
    /// </summary>
    ~ObjectVsBroadPhaseLayerFilter() => Dispose(isDisposing: false);

    protected override void Dispose(bool isDisposing)
    {
        if (isDisposing)
        {
            s_listeners.Remove(Handle);

            JPH_ObjectVsBroadPhaseLayerFilter_Destroy(Handle);
        }
    }

    protected abstract bool ShouldCollide(ObjectLayer layer1, BroadPhaseLayer layer2);

    //[UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
    private static uint ShouldCollideCallback(IntPtr listenerPtr, ObjectLayer layer1, BroadPhaseLayer layer2)
    {
        ObjectVsBroadPhaseLayerFilter listener = s_listeners[listenerPtr];
        return listener.ShouldCollide(layer1, layer2) ? 1u : 0u;
    }
}
