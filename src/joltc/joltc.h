// Copyright � Amer Koleci and Contributors.
// Licensed under the MIT License (MIT). See LICENSE in the repository root for more information.

#ifndef _JOLT_C_H
#define _JOLT_C_H

#if defined(JPH_SHARED_LIBRARY_BUILD)
#   if defined(_MSC_VER)
#       define _JPH_EXPORT __declspec(dllexport)
#   elif defined(__GNUC__)
#       define _JPH_EXPORT __attribute__((visibility("default")))
#   else
#       define _JPH_EXPORT
#       pragma warning "Unknown dynamic link import/export semantics."
#   endif
#elif defined(VIMAGE_SHARED_LIBRARY_INCLUDE)
#   if defined(_MSC_VER)
#       define _JPH_EXPORT __declspec(dllimport)
#   else
#       define _JPH_EXPORT
#   endif
#else
#   define _JPH_EXPORT
#endif

#ifdef __cplusplus
#    define _JPH_EXTERN extern "C"
#else
#    define _JPH_EXTERN extern
#endif

#ifdef _WIN32
#   define JPH_API_CALL __cdecl
#else
#   define JPH_API_CALL
#endif


#define JPH_CAPI _JPH_EXTERN _JPH_EXPORT

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t JPH_BodyID;
typedef uint32_t JPH_SubShapeID;
typedef uint16_t JPH_ObjectLayer;
typedef uint8_t  JPH_BroadPhaseLayer;

typedef bool (JPH_API_CALL* JPH_ObjectVsBroadPhaseLayerFilter)(JPH_ObjectLayer inLayer1, JPH_BroadPhaseLayer inLayer2);
typedef bool (JPH_API_CALL* JPH_ObjectLayerPairFilter)(JPH_ObjectLayer inLayer1, JPH_ObjectLayer inLayer2);

typedef enum JPH_MotionType {
    JPH_MOTION_TYPE_STATIC = 0,
    JPH_MOTION_TYPE_KINEMATIC = 1,
    JPH_MOTION_TYPE_DYNAMIC = 2,

    _JPH_MOTION_TYPE__NUM,
    _JPH_MOTION_TYPE_FORCEU32 = 0x7fffffff
} JPH_MotionType;

typedef enum JPH_ActivationMode
{
    JPH_ACTIVATION_MODE_ACTIVATE = 0,
    JPH_ACTIVATION_MODE_DONT_ACTIVATE = 1,

    _JPH_ACTIVATION_MODE_NUM,
    _JPH_ACTIVATION_MODE_FORCEU32 = 0x7fffffff
} JPH_ActivationMode;

typedef enum JPH_ValidateResult {
    JPH_VALIDATE_RESULT_ACCEPT_ALL_CONTACTS = 0,
    JPH_VALIDATE_RESULT_ACCEPT_CONTACT = 1,
    JPH_VALIDATE_RESULT_REJECT_CONTACT = 2,
    JPH_VALIDATE_RESULT_REJECT_ALL_CONTACTS = 3,

    _JPH_VALIDATE_RESULT_NUM,
    _JPH_VALIDATE_RESULT_FORCEU32 = 0x7fffffff
} JPH_ValidateResult;

typedef struct JPH_Vec3 {
    float x;
    float y;
    float z;
} JPH_Vec3;

typedef struct JPH_Quat {
    float x;
    float y;
    float z;
    float w;
} JPH_Quat;

typedef struct JPH_SubShapeIDPair
{
    JPH_BodyID     Body1ID;
    JPH_SubShapeID subShapeID1;
    JPH_BodyID     Body2ID;
    JPH_SubShapeID subShapeID2;
} JPH_SubShapeIDPair;

typedef struct JPH_TempAllocator JPH_TempAllocator;
typedef struct JPH_JobSystemThreadPool JPH_JobSystemThreadPool;
typedef struct JPH_BroadPhaseLayerInterface JPH_BroadPhaseLayerInterface;
typedef struct JPH_PhysicsSystem JPH_PhysicsSystem;

typedef struct JPH_ShapeSettings JPH_ShapeSettings;
typedef struct JPH_BoxShapeSettings JPH_BoxShapeSettings;
typedef struct JPH_SphereShapeSettings JPH_SphereShapeSettings;

typedef struct JPH_BodyCreationSettings JPH_BodyCreationSettings;
typedef struct JPH_BodyInterface JPH_BodyInterface;
typedef struct JPH_Body JPH_Body;

typedef struct JPH_CollideShapeResult   JPH_CollideShapeResult;
typedef struct JPH_ContactListener      JPH_ContactListener;

JPH_CAPI bool JPH_Init(void);
JPH_CAPI void JPH_Shutdown(void);

/* JPH_TempAllocator */
JPH_CAPI JPH_TempAllocator* JPH_TempAllocator_Create(uint32_t size);
JPH_CAPI void JPH_TempAllocator_Destroy(JPH_TempAllocator* allocator);

/* JPH_JobSystemThreadPool */
JPH_CAPI JPH_JobSystemThreadPool* JPH_JobSystemThreadPool_Create(uint32_t maxJobs, uint32_t maxBarriers, int inNumThreads);
JPH_CAPI void JPH_JobSystemThreadPool_Destroy(JPH_JobSystemThreadPool* system);

/* JPH_BroadPhaseLayer */
JPH_CAPI JPH_BroadPhaseLayerInterface* JPH_BroadPhaseLayer_Create();
JPH_CAPI void JPH_BroadPhaseLayer_Destroy(JPH_BroadPhaseLayerInterface* layer);

/* JPH_ShapeSettings */
JPH_CAPI void JPH_ShapeSettings_Destroy(JPH_ShapeSettings* settings);
JPH_CAPI JPH_BoxShapeSettings* JPH_BoxShapeSettings_Create(const JPH_Vec3* halfExtent, float convexRadius);
JPH_CAPI JPH_SphereShapeSettings* JPH_SphereShapeSettings_Create(float radius);

/* JPH_BodyCreationSettings */
JPH_CAPI JPH_BodyCreationSettings* JPH_BodyCreationSettings_Create();
JPH_CAPI JPH_BodyCreationSettings* JPH_BodyCreationSettings_Create2(JPH_ShapeSettings* settings,
    const JPH_Vec3* position,
    const JPH_Quat* rotation,
    JPH_MotionType motionType,
    JPH_ObjectLayer objectLayer);
JPH_CAPI void JPH_BodyCreationSettings_Destroy(JPH_BodyCreationSettings* settings);

/* JPH_PhysicsSystem */
JPH_CAPI JPH_PhysicsSystem* JPH_PhysicsSystem_Create(void);
JPH_CAPI void JPH_PhysicsSystem_Destroy(JPH_PhysicsSystem* system);
JPH_CAPI void JPH_PhysicsSystem_Init(JPH_PhysicsSystem* system,
    uint32_t maxBodies, uint32_t numBodyMutexes, uint32_t maxBodyPairs, uint32_t maxContactConstraints,
    JPH_BroadPhaseLayer* layer,
    JPH_ObjectVsBroadPhaseLayerFilter objectVsBroadPhaseLayerFilter,
    JPH_ObjectLayerPairFilter objectLayerPairFilter);

JPH_CAPI void JPH_PhysicsSystem_OptimizeBroadPhase(JPH_PhysicsSystem* system);
JPH_CAPI void JPH_PhysicsSystem_Update(JPH_PhysicsSystem* system, float deltaTime, int collisionSteps, int integrationSubSteps,
    JPH_TempAllocator* tempAlocator,
    JPH_JobSystemThreadPool* jobSystem);

JPH_CAPI JPH_BodyInterface* JPH_PhysicsSystem_GetBodyInterface(JPH_PhysicsSystem* system);
JPH_CAPI void JPH_PhysicsSystem_SetContactListener(JPH_PhysicsSystem* system, JPH_ContactListener* listener);

/* BodyInterface */
JPH_CAPI void JPH_BodyInterface_DestroyBody(JPH_BodyInterface* interface, JPH_BodyID bodyID);
JPH_CAPI JPH_BodyID JPH_BodyInterface_CreateAndAddBody(JPH_BodyInterface* interface, JPH_BodyCreationSettings* settings, JPH_ActivationMode activation);
JPH_CAPI JPH_Body* JPH_BodyInterface_CreateBody(JPH_BodyInterface* interface, JPH_BodyCreationSettings* settings);

JPH_CAPI void JPH_BodyInterface_AddBody(JPH_BodyInterface* interface, JPH_BodyID bodyID, JPH_ActivationMode activation);
JPH_CAPI void JPH_BodyInterface_RemoveBody(JPH_BodyInterface* interface, JPH_BodyID bodyID);
JPH_CAPI bool JPH_BodyInterface_IsActive(JPH_BodyInterface* interface, JPH_BodyID bodyID);
JPH_CAPI bool JPH_BodyInterface_IsAdded(JPH_BodyInterface* interface, JPH_BodyID bodyID);

JPH_CAPI void JPH_BodyInterface_SetLinearVelocity(JPH_BodyInterface* interface, JPH_BodyID bodyID, const JPH_Vec3* velocity);
JPH_CAPI void JPH_BodyInterface_GetLinearVelocity(JPH_BodyInterface* interface, JPH_BodyID bodyID, JPH_Vec3* velocity);
JPH_CAPI void JPH_BodyInterface_GetCenterOfMassPosition(JPH_BodyInterface* interface, JPH_BodyID bodyID, JPH_Vec3* position);

JPH_CAPI JPH_MotionType JPH_BodyInterface_GetMotionType(JPH_BodyInterface* interface, JPH_BodyID bodyID);
JPH_CAPI void JPH_BodyInterface_SetMotionType(JPH_BodyInterface* interface, JPH_BodyID bodyID, JPH_MotionType motionType, JPH_ActivationMode activationMode);

/* Body */
JPH_CAPI JPH_BodyID JPH_Body_GetID(JPH_Body* body);
JPH_CAPI bool JPH_Body_IsActive(JPH_Body* body);
JPH_CAPI bool JPH_Body_IsStatic(JPH_Body* body);
JPH_CAPI bool JPH_Body_IsKinematic(JPH_Body* body);
JPH_CAPI bool JPH_Body_IsDynamic(JPH_Body* body);
JPH_CAPI bool JPH_Body_IsSensor(JPH_Body* body);

JPH_CAPI JPH_MotionType JPH_Body_GetMotionType(JPH_Body* body);
JPH_CAPI void JPH_Body_SetMotionType(JPH_Body* body, JPH_MotionType motionType);

/* Contact listener */
typedef struct JPH_ContactListener_Procs {
    JPH_ValidateResult (JPH_API_CALL *OnContactValidate)(JPH_ContactListener* listener,
        const JPH_Body* body1,
        const JPH_Body* body2,
        const JPH_CollideShapeResult* collisionResult);

    void(JPH_API_CALL* OnContactAdded)(JPH_ContactListener* listener,
        const JPH_Body* body1,
        const JPH_Body* body2);

    void(JPH_API_CALL* OnContactPersisted)(JPH_ContactListener* listener,
        const JPH_Body* body1,
        const JPH_Body* body2);

    void(JPH_API_CALL* OnContactRemoved)(JPH_ContactListener* listener,
        const JPH_SubShapeIDPair* subShapePair
        );
} JPH_ContactListener_Procs;

JPH_CAPI void JPH_ContactListener_SetProcs(JPH_ContactListener_Procs procs);
JPH_CAPI JPH_ContactListener* JPH_ContactListener_Create();
JPH_CAPI void JPH_ContactListener_Destroy(JPH_ContactListener* listener);

#endif /* _JOLT_C_H */
