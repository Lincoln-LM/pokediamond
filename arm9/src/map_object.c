#include "global.h"
#include "map_object.h"
#include "field_system.h"
#include "heap.h"
#include "constants/sprites.h"
#include "unk_0200CA44.h"

static MapObjectManager *MapObjectManager_New(u32 objectCount);
static LocalMapObject *MapObject_CreateFromObjectEvent(MapObjectManager *manager, ObjectEvent *objectEvent, u32 mapNo);
static void SavedMapObject_InitFromLocalMapObject(FieldSystem *fieldSystem, LocalMapObject *object, SavedMapObject *savedObject);
static void LocalMapObject_InitFromSavedMapObject(LocalMapObject *localObject, SavedMapObject *savedObject);
static void sub_02057AEC(MapObjectManager *manager, LocalMapObject *object);
static void sub_02057B34(LocalMapObject *object);
static void MapObject_ConvertXYToPositionVec(LocalMapObject *object);
static void MapObject_CreateFromInitArgs(MapObjectInitArgs *args);
static LocalMapObject *MapObjectManager_GetFirstInactiveObject(MapObjectManager *manager);
static LocalMapObject *sub_02057C98(MapObjectManager *manager, u32 id, u32 mapNo);
static void sub_02057CF0(MapObjectManager *manager, LocalMapObject *object);
static void MapObject_InitFromObjectEvent(LocalMapObject *object, ObjectEvent *objectEvent, FieldSystem *fieldSystem);
static void MapObject_SetPositionVecFromObjectEvent(LocalMapObject *object, ObjectEvent *objectEvent);
static void sub_02057E90(LocalMapObject *object, MapObjectManager *manager);
/*static*/ void sub_02057EE0(LocalMapObject *object);
/*static*/ void sub_02057F18(LocalMapObject *object);
static void MapObject_Clear(LocalMapObject *object);
static u32 sub_02057F90(LocalMapObject *object, u32 flagId, u32 objectEventCount, ObjectEvent *objectEvents);
static LocalMapObject *MapObjectManager_GetFirstObjectWithID(MapObjectManager *manager, u32 objectId, u32 flagId);
static void sub_02058148(LocalMapObject *object);
/*static*/ void sub_0205815C(LocalMapObject *object);
static void MapObject_NoOp(LocalMapObject *object);
static u32 FieldSystem_ResolveObjectSpriteID(FieldSystem *fieldSystem, s32 spriteId);
/*static*/ void sub_0205818C(LocalMapObject *object);
static void sub_020581A4(LocalMapObject *object);

extern void MapObjectManager_SetFieldSystemPtr(MapObjectManager *manager, FieldSystem *fieldSystem);
extern void MapObjectManager_SetObjectCount(MapObjectManager *manager, u32 objectCount);
extern void MapObjectManager_SetPriority(MapObjectManager *manager, u32 priority);
extern LocalMapObject *MapObjectManager_GetObjects(MapObjectManager *manager);
extern u32 MapObjectManager_GetObjectCount(MapObjectManager *manager);
extern BOOL MapObject_IsInUse(LocalMapObject *object);
extern void ov05_021F2AF4(MapObjectManager *manager, void *param0);
extern void *sub_020583A0(MapObjectManager *manager);
extern u32 sub_02058450(LocalMapObject *object);
extern BOOL MapObject_TestFlagsBits(LocalMapObject *object, MapObjectFlagBits flags);
extern void MapObjectManager_SetObjects(MapObjectManager *manager, LocalMapObject *objects);
extern u32 ObjectEvent_GetID(ObjectEvent *objectEvent);
extern BOOL ObjectEvent_ScriptIDIsUnset(ObjectEvent *objectEvent);
extern void sub_02058214(LocalMapObject *object, ObjectEvent *objectEvent, u32 mapNo);
extern u16 ObjectEvent_GetFlagID_AssertScriptIDIsUnset(ObjectEvent *objectEvent);
extern void sub_02058258(LocalMapObject *object, u32 mapNo, ObjectEvent *objectEvent);
extern FieldSystem *MapObjectManager_GetFieldSystemPtr(MapObjectManager *manager);
extern void sub_02057E90(LocalMapObject *object, MapObjectManager *manager);
extern void sub_0205844C(LocalMapObject *object, u32 mapNo);
extern void sub_020581B4(LocalMapObject *object);
extern void MapObject_SetFlagsBits(LocalMapObject *object, MapObjectFlagBits bits);
extern MapObjectManager *MapObjectManager_GetMapObjectManager(MapObjectManager *manager);
extern void sub_0205836C(MapObjectManager *manager);
extern void ObjectEvent_SetId(ObjectEvent *objectEvent, u32 id);
extern void ObjectEvent_SetSprite(ObjectEvent *objectEvent, u32 sprite);
extern void ObjectEvent_SetMovement(ObjectEvent *objectEvent, u32 movement);
extern void ObjectEvent_SetType(ObjectEvent *objectEvent, u32 type);
extern void ObjectEvent_SetFlagId(ObjectEvent *objectEvent, u32 flagId);
extern void ObjectEvent_SetScript(ObjectEvent *objectEvent, u32 script);
extern void ObjectEvent_SetFacing(ObjectEvent *objectEvent, u32 direction);
extern void ObjectEvent_SetParam(ObjectEvent *objectEvent, u32 value, u32 param);
extern void ObjectEvent_SetXRange(ObjectEvent *objectEvent, u32 xRange);
extern void ObjectEvent_SetYRange(ObjectEvent *objectEvent, u32 yRange);
extern void ObjectEvent_SetXCoord(ObjectEvent *objectEvent, u32 xCoord);
extern void ObjectEvent_SetYCoord(ObjectEvent *objectEvent, u32 yCoord);
extern void ObjectEvent_SetHeight(ObjectEvent *objectEvent, u32 height);
extern u32 ObjectEvent_GetFlagID(ObjectEvent *objectEvent);
extern ObjectEvent *ObjectEvent_GetById(u32 id, u32 objectEventCount, ObjectEvent *events);
extern u8 FieldSystem_FlagCheck(FieldSystem *fieldSystem, u16 flag);
extern void MapObject_SetSpriteID(LocalMapObject *object, u32 sprite);
extern void MapObject_ClearFlagsBits(LocalMapObject *object, MapObjectFlagBits bits);
extern BOOL sub_02058934(LocalMapObject *object);
extern MapObjectManager *MapObject_GetManager(LocalMapObject *object);
extern BOOL sub_020587E0(MapObjectManager *manager);
extern void sub_020586B4(LocalMapObject *object);
extern void sub_02058660(LocalMapObject *object);
extern void sub_02058564(LocalMapObject *object);
extern MapObjectManager *sub_02058580(LocalMapObject *object);
extern void sub_02058374(MapObjectManager *manager);
extern u32 MapObject_GetFlagID(LocalMapObject *object);
extern FieldSystem *MapObject_GetFieldSystemPtr(LocalMapObject *object);
extern void FieldSystem_FlagSet(FieldSystem *fieldSystem, u16 flag);
extern void sub_02058ED8(LocalMapObject *object);
extern void sub_02058EDC(LocalMapObject *object);
extern void sub_02058EE0(LocalMapObject *object);
extern void sub_02058EE4(LocalMapObject *object);
extern u32 MapObject_GetFlagsBits(LocalMapObject *object, MapObjectFlagBits bits);
extern void sub_02058684(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern void sub_02058698(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern void sub_020586AC(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern void sub_020586C0(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern void sub_020586D4(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern void sub_020586C8(LocalMapObject *object);
extern BOOL MapObject_CheckFlag14(LocalMapObject *object);
extern void sub_020586DC(LocalMapObject *object);
extern void sub_02057AEC(MapObjectManager *manager, LocalMapObject *object);
extern u32 MapObject_GetFlagsWord(LocalMapObject *object);
extern u32 MapObject_GetFlags2Word(LocalMapObject *object);
extern u32 MapObject_GetID(LocalMapObject *object);
extern u32 MapObject_GetSpriteID(LocalMapObject *object);
extern u32 MapObject_GetMovement(LocalMapObject *object);
extern u32 MapObject_GetType(LocalMapObject *object);
extern u32 MapObject_GetScript(LocalMapObject *object);
extern u32 MapObject_GetInitialFacingDirection(LocalMapObject *object);
extern u32 MapObject_GetFacingDirection(LocalMapObject *object);
extern u32 MapObject_GetNextFacingDirection(LocalMapObject *object);
extern u32 MapObject_GetParam(LocalMapObject *object, u32 param);
extern u32 MapObject_GetXRange(LocalMapObject *object);
extern u32 MapObject_GetYRange(LocalMapObject *object);
extern u32 MapObject_GetInitialX(LocalMapObject *object);
extern u32 MapObject_GetInitialHeight(LocalMapObject *object);
extern u32 MapObject_GetInitialY(LocalMapObject *object);
extern u32 MapObject_GetCurrentX(LocalMapObject *object);
extern u32 MapObject_GetCurrentHeight(LocalMapObject *object);
extern u32 MapObject_GetCurrentY(LocalMapObject *object);
extern void sub_02059E60(s32 x, s32 y, VecFx32 *vector);
extern u32 MapObject_GetPosVecYCoord(LocalMapObject *object);
extern u32 MapObject_CheckFlag29(LocalMapObject *object);
extern u8 *sub_020585B0(LocalMapObject *object);
extern u8 *sub_020585D8(LocalMapObject *object);
extern BOOL sub_02059EC8(FieldSystem *fieldSystem, VecFx32 *vector, BOOL flag);
extern void MapObject_SetFlagsWord(LocalMapObject *object, u32 flags);
extern void MapObject_SetFlags2Word(LocalMapObject *object, u32 flags2);
extern void MapObject_SetID(LocalMapObject *object, u32 objectId);
extern void sub_0205844C(LocalMapObject *object, u32 param1);
extern void MapObject_SetMovement(LocalMapObject *object, u32 movement);
extern void MapObject_SetType(LocalMapObject *object, u32 type);
extern void MapObject_SetFlagID(LocalMapObject *object, u32 flagId);
extern void MapObject_SetScript(LocalMapObject *object, u32 script);
extern void MapObject_SetInitialFacingDirection(LocalMapObject *object, u32 initialDirection);
extern void MapObject_SetFacingDirectionDirect(LocalMapObject *object, u32 direction);
extern void MapObject_SetNextFacingDirection(LocalMapObject *object, u32 nextDirection);
extern void MapObject_SetParam(LocalMapObject *object, u32 value, u32 param);
extern void MapObject_SetXRange(LocalMapObject *object, u32 xRange);
extern void MapObject_SetYRange(LocalMapObject *object, u32 yRange);
extern void MapObject_SetInitialX(LocalMapObject *object, u32 initialX);
extern void MapObject_SetInitialHeight(LocalMapObject *object, u32 initialHeight);
extern void MapObject_SetInitialY(LocalMapObject *object, u32 initialY);
extern void MapObject_SetCurrentX(LocalMapObject *object, u32 currentX);
extern void MapObject_SetCurrentHeight(LocalMapObject *object, u32 currentHeight);
extern void MapObject_SetCurrentY(LocalMapObject *object, u32 currentY);
extern void MapObject_SetPositionVec(LocalMapObject *object, VecFx32 *coords);
extern void MapObject_SetManager(LocalMapObject *object, MapObjectManager *manager);
extern void MapObject_ClearHeldMovement(LocalMapObject *object);
extern void sub_0205866C(LocalMapObject *object);
extern void MapObject_GetPositionVec(LocalMapObject *object, VecFx32 *position);
extern void MapObject_SetPreviousX(LocalMapObject *object, u32 previousX);
extern void MapObject_SetPreviousHeight(LocalMapObject *object, u32 previousHeight);
extern void MapObject_SetPreviousY(LocalMapObject *object, u32 previousY);
extern void MapObject_CreateFromInitArgs(MapObjectInitArgs *args);
extern BOOL MapObject_CheckFlag25(LocalMapObject *object);
extern u32 sub_02058750(LocalMapObject *object);
extern void sub_0205832C(SysTask *task, LocalMapObject *object);
extern u32 MapObjectManager_GetPriority(MapObjectManager *manager);
extern void sub_02058554(LocalMapObject *object, SysTask *task);
extern u16 ObjectEvent_GetSpriteID(ObjectEvent *objectEvent);
extern u16 ObjectEvent_GetMovement(ObjectEvent *objectEvent);
extern u16 ObjectEvent_GetType(ObjectEvent *objectEvent);
extern u16 ObjectEvent_GetScript(ObjectEvent *objectEvent);
extern u16 ObjectEvent_GetInitialFacingDirection(ObjectEvent *objectEvent);
extern u32 ObjectEvent_GetParam(ObjectEvent *objectEvent, u32 param);
extern u32 ObjectEvent_GetXRange(ObjectEvent *objectEvent);
extern u32 ObjectEvent_GetYRange(ObjectEvent *objectEvent);
extern u16 ObjectEvent_GetXCoord(ObjectEvent *objectEvent);
extern u16 ObjectEvent_GetYCoord(ObjectEvent *objectEvent);
extern u32 ObjectEvent_GetHeight(ObjectEvent *objectEvent);
extern BOOL MapObject_ScriptIdIsFFFF(LocalMapObject *object);
extern void MapObject_SetFlag25(LocalMapObject *object, BOOL flag);
extern UnkLMOCallbackStruct *sub_02058D14(u32 movement);
extern LocalMapObject_UnkCallback sub_02058D2C(UnkLMOCallbackStruct *callbackStruct);
extern void sub_02058630(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern LocalMapObject_UnkCallback sub_02058D30(UnkLMOCallbackStruct *callbackStruct);
extern void sub_02058644(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern LocalMapObject_UnkCallback sub_02058D34(UnkLMOCallbackStruct *callbackStruct);
extern void sub_02058658(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern UnkLMOCallbackStruct2 *sub_02058D4C(u32 spriteId);
extern LocalMapObject_UnkCallback sub_02058D38(UnkLMOCallbackStruct2 *callbackStruct);
extern void sub_02058684(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern LocalMapObject_UnkCallback sub_02058D3C(UnkLMOCallbackStruct2 *callbackStruct);
extern void sub_02058698(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern LocalMapObject_UnkCallback sub_02058D40(UnkLMOCallbackStruct2 *callbackStruct);
extern void sub_020586AC(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern LocalMapObject_UnkCallback sub_02058D44(UnkLMOCallbackStruct2 *callbackStruct);
extern void sub_020586C0(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern LocalMapObject_UnkCallback sub_02058D48(UnkLMOCallbackStruct2 *callbackStruct);
extern void sub_020586D4(LocalMapObject *object, LocalMapObject_UnkCallback callback);
extern const LocalMapObject *MapObjectManager_GetConstObjects(MapObjectManager *manager);
extern u16 FieldSystem_VarGetObjectEventGraphicsId(FieldSystem *fieldSystem, u16 spriteId);
extern u32 sub_02059D1C(LocalMapObject *object);
extern void sub_02058EE8(LocalMapObject *object);

MapObjectManager *MapObjectManager_Init(FieldSystem *fieldSystem, u32 objectCount, u32 priority) {
    MapObjectManager *ret = MapObjectManager_New(objectCount);
    MapObjectManager_SetFieldSystemPtr(ret, fieldSystem);
    MapObjectManager_SetObjectCount(ret, objectCount);
    MapObjectManager_SetPriority(ret, priority);

    return ret;
}

void MapObjectManager_Delete(MapObjectManager *manager) {
    FreeToHeapExplicit(HEAP_ID_FIELD, MapObjectManager_GetObjects(manager));
    FreeToHeapExplicit(HEAP_ID_FIELD, manager);
}

void sub_020573C8(MapObjectManager *manager, u32 unused, u32 flagId, u32 objectCount, ObjectEvent *objectEvents) {
    u32 count = MapObjectManager_GetObjectCount(manager);
    LocalMapObject *objects = MapObjectManager_GetObjects(manager);

    for (; count != 0; count--) {
        if (MapObject_IsInUse(objects) == TRUE) {
            switch (sub_02057F90(objects, flagId, objectCount, objectEvents)) {
                case 0:
                    if (sub_02058450(objects) != flagId && !MapObject_TestFlagsBits(objects, MAPOBJECTFLAG_UNK10)) {
                        MapObject_Remove(objects);
                    }
                    break;

                case 1:
                case 2:
                    break;
            }
        }
        objects++;
    }

    ov05_021F2AF4(manager, sub_020583A0(manager));
}

static MapObjectManager *MapObjectManager_New(u32 objectCount) {
    LocalMapObject *objects;
    MapObjectManager *manager = AllocFromHeap(HEAP_ID_FIELD, sizeof(MapObjectManager));
    GF_ASSERT(manager != NULL);
    memset(manager, 0, sizeof(MapObjectManager));

    objects = AllocFromHeap(HEAP_ID_FIELD, objectCount * sizeof(LocalMapObject));
    GF_ASSERT(objects != NULL);
    memset(objects, 0, objectCount * sizeof(LocalMapObject));

    MapObjectManager_SetObjects(manager, objects);

    return manager;
}

static LocalMapObject *MapObject_CreateFromObjectEvent(MapObjectManager *manager, ObjectEvent *objectEvent, u32 mapNo) {
    LocalMapObject *ret;

    u32 objectId = ObjectEvent_GetID(objectEvent);
    if (!ObjectEvent_ScriptIDIsUnset(objectEvent)) {
        ret = sub_02057C98(manager, objectId, mapNo);
        if (ret != NULL) {
            sub_02058214(ret, objectEvent, mapNo);
            return ret;
        }
    } else {
        ret = MapObjectManager_GetFirstObjectWithID(manager, objectId, ObjectEvent_GetFlagID_AssertScriptIDIsUnset(objectEvent));
        if (ret != NULL) {
            sub_02058258(ret, mapNo, objectEvent);
            return ret;
        }
    }
    
    ret = MapObjectManager_GetFirstInactiveObject(manager);
    if (ret == NULL) {
        return ret;
    }

    MapObject_InitFromObjectEvent(ret, objectEvent, MapObjectManager_GetFieldSystemPtr(manager));
    sub_02057E90(ret, manager);
    sub_0205844C(ret, mapNo);
    sub_020581A4(ret);
    sub_020581B4(ret);
    MapObject_SetFlagsBits(ret, MAPOBJECTFLAG_UNK2);
    sub_02057CF0(manager, ret);
    sub_0205836C(MapObjectManager_GetMapObjectManager(manager));
    return ret;
}

LocalMapObject *MapObject_Create(MapObjectManager *manager, u32 x, u32 y, u32 direction, u32 sprite, u32 movement, u32 mapNo) {
    ObjectEvent objectEvent;
    ObjectEvent_SetId(&objectEvent, 0);
    ObjectEvent_SetSprite(&objectEvent, sprite);
    ObjectEvent_SetMovement(&objectEvent, movement);
    ObjectEvent_SetType(&objectEvent, 0);
    ObjectEvent_SetFlagId(&objectEvent, 0);
    ObjectEvent_SetScript(&objectEvent, 0);
    ObjectEvent_SetFacing(&objectEvent, direction);
    ObjectEvent_SetParam(&objectEvent, 0, 0);
    ObjectEvent_SetParam(&objectEvent, 0, 1);
    ObjectEvent_SetParam(&objectEvent, 0, 2);
    ObjectEvent_SetXRange(&objectEvent, 0);
    ObjectEvent_SetYRange(&objectEvent, 0);
    ObjectEvent_SetXCoord(&objectEvent, x);
    ObjectEvent_SetYCoord(&objectEvent, y);
    ObjectEvent_SetHeight(&objectEvent, 0);

    return MapObject_CreateFromObjectEvent(manager, &objectEvent, mapNo);
}

LocalMapObject *MapObject_CreateFromObjectEventWithId(MapObjectManager *manager, u16 id, u32 objectEventCount, u32 mapNo, const ObjectEvent *events) {
    LocalMapObject *ret = NULL;
    ObjectEvent *objectEvent = ObjectEvent_GetById(id, objectEventCount, (ObjectEvent *)events);
    if (objectEvent != NULL) {
        u32 flagId = ObjectEvent_GetFlagID(objectEvent);
        FieldSystem* fieldSystem = MapObjectManager_GetFieldSystemPtr(manager);
        if (FieldSystem_FlagCheck(fieldSystem, (u16)flagId) == FALSE) {
            ret = MapObject_CreateFromObjectEvent(manager, objectEvent, mapNo);
        }
    }

    return ret;
}

void sub_02057614(LocalMapObject *object, u32 sprite) {
    MapObject_SetSpriteID(object, sprite);
    sub_02058148(object);
    MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK14);
    sub_020581B4(object);
}

void sub_02057634(LocalMapObject *object, u32 sprite) {
    if (sub_02058934(object) == TRUE) {
        sub_020576A8(object);
    }
    sub_02057614(object, sprite);
}

void MapObject_Remove(LocalMapObject *object) {
    if (sub_020587E0(MapObject_GetManager(object)) == TRUE) {
        sub_020586B4(object);
    }
    sub_02058660(object);
    sub_02058564(object);
    sub_02058374(sub_02058580(object));
    MapObject_Clear(object);
}

void MapObject_Delete(LocalMapObject *object) {
    u32 flagId = MapObject_GetFlagID(object);
    FieldSystem *fieldSystem = MapObject_GetFieldSystemPtr(object);
    FieldSystem_FlagSet(fieldSystem, (u16)flagId);
    MapObject_Remove(object);
}

void sub_020576A8(LocalMapObject *object) {
    if (sub_020587E0(MapObject_GetManager(object)) == TRUE) {
        if (MapObject_GetFlagsBits(object, MAPOBJECTFLAG_UNK14) != 0) {
            sub_020586B4(object);
        }
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK14);
    }
    MapObject_SetSpriteID(object, MAP_OBJECT_GFX_ID_INVALID);
    sub_02058684(object, sub_02058ED8);
    sub_02058698(object, sub_02058EDC);
    sub_020586AC(object, sub_02058EDC);
    sub_020586C0(object, sub_02058EE0);
    sub_020586D4(object, sub_02058EE4);
}

void MapObjectManager_RemoveAllActiveObjects(MapObjectManager *manager) {
    s32 i = 0;
    s32 count = MapObjectManager_GetObjectCount(manager);
    LocalMapObject *objects = MapObjectManager_GetObjects(manager);

    do { //BUG: does not check for if count is zero, and will always execute at least once
        if (MapObject_GetFlagsBits(objects, MAPOBJECTFLAG_ACTIVE) != 0) {
            MapObject_Remove(objects);
        }

        i++;
        objects++;
    } while (i < count);
}

void sub_02057750(MapObjectManager *manager) {
    GF_ASSERT(sub_020587E0(manager) == TRUE);

    s32 i = 0;
    s32 count = MapObjectManager_GetObjectCount(manager);
    LocalMapObject *objects = MapObjectManager_GetObjects(manager);

    do {
        if (MapObject_GetFlagsBits(objects, MAPOBJECTFLAG_ACTIVE) != 0 && MapObject_GetFlagsBits(objects, MAPOBJECTFLAG_UNK14) != 0) {
            sub_020586C8(objects);
            MapObject_NoOp(objects);
        }

        i++;
        objects++;
    } while (i < count);
}

void sub_020577A8(MapObjectManager *manager) {
    GF_ASSERT(sub_020587E0(manager) == TRUE);

    s32 i = 0;
    s32 count = MapObjectManager_GetObjectCount(manager);
    LocalMapObject *objects = MapObjectManager_GetObjects(manager);

    do {
        if (MapObject_IsInUse(objects) == TRUE) {
            if (MapObject_CheckFlag14(objects) == TRUE) {
                sub_020586DC(objects);
            } else {
                sub_020581B4(objects);
            }

            sub_02058148(objects);
        }

        i++;
        objects++;
    } while (i < count);
}

void FieldSystem_SyncMapObjectsToSaveEx(FieldSystem *fieldSystem, MapObjectManager *manager, SavedMapObject *savedObjects, s32 count) {
    s32 index = 0;
    LocalMapObject *object;

    while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE)) { //MapObjectManager_GetNextActiveObject? this is an iterator however
        SavedMapObject_InitFromLocalMapObject(fieldSystem, object, savedObjects);

        count--;
        savedObjects++;

        GF_ASSERT(count > 0);
    }

    if (count != 0) {
        memset(savedObjects, 0, count * sizeof(SavedMapObject));
    }
}

void MapObjectManager_RestoreFromSave(MapObjectManager *manager, SavedMapObject *savedObjects, u32 objectCount) {
    while (objectCount > 0) {
        if ((savedObjects->flags & MAPOBJECTFLAG_ACTIVE)) {
            LocalMapObject *localObject = MapObjectManager_GetFirstInactiveObject(manager);
            GF_ASSERT(localObject != NULL);

            LocalMapObject_InitFromSavedMapObject(localObject, savedObjects);
            sub_02057AEC(manager, localObject);
        }
        savedObjects++;
        objectCount--;
    }
}

static void SavedMapObject_InitFromLocalMapObject(FieldSystem *fieldSystem, LocalMapObject *localObject, SavedMapObject *savedObject) {
    savedObject->flags = MapObject_GetFlagsWord(localObject);
    savedObject->flags2 = MapObject_GetFlags2Word(localObject);
    savedObject->objId = MapObject_GetID(localObject);
    savedObject->unk10 = sub_02058450(localObject);
    savedObject->spriteId = MapObject_GetSpriteID(localObject);
    savedObject->movement = MapObject_GetMovement(localObject);
    savedObject->type = MapObject_GetType(localObject);
    savedObject->flagId = MapObject_GetFlagID(localObject);
    savedObject->script = MapObject_GetScript(localObject);
    savedObject->initialFacing = MapObject_GetInitialFacingDirection(localObject);
    savedObject->currentFacing = MapObject_GetFacingDirection(localObject);
    savedObject->nextFacing = MapObject_GetNextFacingDirection(localObject);
    savedObject->param[0] = MapObject_GetParam(localObject, 0);
    savedObject->param[1] = MapObject_GetParam(localObject, 1);
    savedObject->param[2] = MapObject_GetParam(localObject, 2);
    savedObject->xRange = MapObject_GetXRange(localObject);
    savedObject->yRange = MapObject_GetYRange(localObject);
    savedObject->initialX = MapObject_GetInitialX(localObject);
    savedObject->initialHeight = MapObject_GetInitialHeight(localObject);
    savedObject->initialY = MapObject_GetInitialY(localObject);
    savedObject->currentX = MapObject_GetCurrentX(localObject);
    savedObject->currentHeight = MapObject_GetCurrentHeight(localObject);
    savedObject->currentY = MapObject_GetCurrentY(localObject);

    VecFx32 coords;
    sub_02059E60(savedObject->currentX, savedObject->currentY, &coords); //some kind of x y vec copy with convertion between int and fx32
    coords.y = MapObject_GetPosVecYCoord(localObject);

    if (!sub_02059EC8(fieldSystem, &coords, MapObject_CheckFlag29(localObject))) {
        savedObject->vecY = MapObject_GetPosVecYCoord(localObject);
    } else {
        savedObject->vecY = coords.y;
    }
    
    memcpy(savedObject->unk30, sub_020585B0(localObject), 16);
    memcpy(savedObject->unk40, sub_020585D8(localObject), 16);
}

static void LocalMapObject_InitFromSavedMapObject(LocalMapObject *localObject, SavedMapObject *savedObject) {
    MapObject_SetFlagsWord(localObject, savedObject->flags);
    MapObject_SetFlags2Word(localObject, savedObject->flags2);
    MapObject_SetID(localObject, savedObject->objId);
    sub_0205844C(localObject, savedObject->unk10);
    MapObject_SetSpriteID(localObject, savedObject->spriteId);
    MapObject_SetMovement(localObject, savedObject->movement);
    MapObject_SetType(localObject, savedObject->type);
    MapObject_SetFlagID(localObject, savedObject->flagId);
    MapObject_SetScript(localObject, savedObject->script);
    MapObject_SetInitialFacingDirection(localObject, savedObject->initialFacing);
    MapObject_SetFacingDirectionDirect(localObject, savedObject->currentFacing);
    MapObject_SetNextFacingDirection(localObject, savedObject->nextFacing);
    MapObject_SetParam(localObject, savedObject->param[0], 0);
    MapObject_SetParam(localObject, savedObject->param[1], 1);
    MapObject_SetParam(localObject, savedObject->param[2], 2);
    MapObject_SetXRange(localObject, savedObject->xRange);
    MapObject_SetYRange(localObject, savedObject->yRange);
    MapObject_SetInitialX(localObject, savedObject->initialX);
    MapObject_SetInitialHeight(localObject, savedObject->initialHeight);
    MapObject_SetInitialY(localObject, savedObject->initialY);
    MapObject_SetCurrentX(localObject, savedObject->currentX);
    MapObject_SetCurrentHeight(localObject, savedObject->currentHeight);
    MapObject_SetCurrentY(localObject, savedObject->currentY);

    VecFx32 coords = {};
    coords.y = savedObject->vecY;
    MapObject_SetPositionVec(localObject, &coords);

    memcpy(sub_020585B0(localObject), savedObject->unk30, 16);
    memcpy(sub_020585D8(localObject), savedObject->unk40, 16);
}

static void sub_02057AEC(MapObjectManager *manager, LocalMapObject *object) {
    sub_02057B34(object);
    MapObject_ConvertXYToPositionVec(object);
    MapObject_SetManager(object, manager);
    sub_02057EE0(object);
    MapObject_ClearHeldMovement(object);
    sub_020581B4(object);
    sub_02057CF0(manager, object);
    sub_0205866C(object);
    sub_0205836C(MapObjectManager_GetMapObjectManager(manager));
}

static void sub_02057B34(LocalMapObject *object) {
    MapObject_SetFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK2 | MAPOBJECTFLAG_ACTIVE));
    MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_IGNORE_HEIGHTS | MAPOBJECTFLAG_UNK22 |
                                                         MAPOBJECTFLAG_UNK21 | MAPOBJECTFLAG_UNK19 | MAPOBJECTFLAG_UNK18 |
                                                         MAPOBJECTFLAG_UNK17 | MAPOBJECTFLAG_UNK16 | MAPOBJECTFLAG_UNK14 |
                                                         MAPOBJECTFLAG_VISIBLE | MAPOBJECTFLAG_MOVEMENT_PAUSED | MAPOBJECTFLAG_UNK3));
    sub_0205815C(object);
}

static void MapObject_ConvertXYToPositionVec(LocalMapObject *object) {
    VecFx32 position;
    MapObject_GetPositionVec(object, &position);

    u32 x = MapObject_GetCurrentX(object);
    position.x = x * FX32_CONST(16) + FX32_CONST(8);
    MapObject_SetPreviousX(object, x);

    MapObject_SetPreviousHeight(object, MapObject_GetCurrentHeight(object));

    u32 y = MapObject_GetCurrentY(object);
    position.z = y * FX32_CONST(16) + FX32_CONST(8);
    MapObject_SetPreviousY(object, y);

    MapObject_SetPositionVec(object, &position);
}

void MapObject_CreateFromMultipleObjectEvents(MapObjectManager *manager, u32 mapNo, u32 objectEventCount, ObjectEvent *objectEvents) {
    GF_ASSERT(objectEventCount != 0);

    ObjectEvent *objectEventsCopy = AllocFromHeapAtEnd(HEAP_ID_FIELD, objectEventCount * sizeof(ObjectEvent));
    GF_ASSERT(objectEventsCopy != NULL);
    memcpy(objectEventsCopy, objectEvents, objectEventCount * sizeof(ObjectEvent));

    MapObjectInitArgs *args = AllocFromHeapAtEnd(HEAP_ID_FIELD, sizeof(MapObjectInitArgs));
    GF_ASSERT(args != NULL);
    args->mapNo = mapNo;
    args->objectEventCount = objectEventCount;
    args->index = 0;
    args->manager = manager;
    args->objectEvents = objectEventsCopy;

    MapObject_CreateFromInitArgs(args);
}

static void MapObject_CreateFromInitArgs(MapObjectInitArgs *args) {
    FieldSystem *fieldSystem = MapObjectManager_GetFieldSystemPtr(args->manager);
    ObjectEvent *objectEvents = args->objectEvents;

    do { //must be do while due to initial check
        GF_ASSERT((ObjectEvent_ScriptIDIsUnset(objectEvents) != TRUE && FieldSystem_FlagCheck(fieldSystem, objectEvents->flag) != FALSE) ||
                  MapObject_CreateFromObjectEvent(args->manager, objectEvents, args->mapNo) != NULL); //this is dumb, but is required to match

        objectEvents++;
        args->index++;
    } while (args->index < args->objectEventCount);

    FreeToHeapExplicit(HEAP_ID_FIELD, args->objectEvents);
    FreeToHeapExplicit(HEAP_ID_FIELD, args);
}

static LocalMapObject *MapObjectManager_GetFirstInactiveObject(MapObjectManager *manager) {
    s32 i = 0;
    s32 count = MapObjectManager_GetObjectCount(manager);
    LocalMapObject *objects = MapObjectManager_GetObjects(manager);

    do {
        if (MapObject_GetFlagsBits(objects, MAPOBJECTFLAG_ACTIVE) == 0) {
            return objects;
        }

        i++;
        objects++;
    } while (i < count);

    return NULL;
}

static LocalMapObject *sub_02057C98(MapObjectManager *manager, u32 id, u32 mapNo) { //find corresponding LMO with ID and mapNo?
    s32 index = 0;
    LocalMapObject *object;

    while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        if (MapObject_CheckFlag25(object) == TRUE && MapObject_GetID(object) == id && sub_02058750(object) == mapNo) {
            return object;
        }
    }

    return NULL;
}

static void sub_02057CF0(MapObjectManager *manager, LocalMapObject *object) {
    u32 priority = MapObjectManager_GetPriority(manager);
    u32 movement = MapObject_GetMovement(object);
    if (movement == 48 || movement == 50) {
        priority += 2;
    }

    SysTask *task = SysTask_CreateOnMainQueue((SysTaskFunc)sub_0205832C, object, priority);
    GF_ASSERT(task != NULL);

    sub_02058554(object, task);
}

static void MapObject_InitFromObjectEvent(LocalMapObject *object, ObjectEvent *objectEvent, FieldSystem *fieldSystem) {
    MapObject_SetID(object, ObjectEvent_GetID(objectEvent));
    MapObject_SetSpriteID(object, FieldSystem_ResolveObjectSpriteID(fieldSystem, ObjectEvent_GetSpriteID(objectEvent)));
    MapObject_SetMovement(object, ObjectEvent_GetMovement(objectEvent));
    MapObject_SetType(object, ObjectEvent_GetType(objectEvent));
    MapObject_SetFlagID(object, ObjectEvent_GetFlagID(objectEvent));
    MapObject_SetScript(object, ObjectEvent_GetScript(objectEvent));
    MapObject_SetInitialFacingDirection(object, ObjectEvent_GetInitialFacingDirection(objectEvent));
    MapObject_SetParam(object, ObjectEvent_GetParam(objectEvent, 0), 0);
    MapObject_SetParam(object, ObjectEvent_GetParam(objectEvent, 1), 1);
    MapObject_SetParam(object, ObjectEvent_GetParam(objectEvent, 2), 2);
    MapObject_SetXRange(object, ObjectEvent_GetXRange(objectEvent));
    MapObject_SetYRange(object, ObjectEvent_GetYRange(objectEvent));

    MapObject_SetPositionVecFromObjectEvent(object, objectEvent);
}

static void MapObject_SetPositionVecFromObjectEvent(LocalMapObject *object, ObjectEvent *objectEvent) {
    VecFx32 coords;

    u16 x = ObjectEvent_GetXCoord(objectEvent);
    coords.x = x * FX32_CONST(16) + FX32_CONST(8);
    MapObject_SetInitialX(object, x);
    MapObject_SetPreviousX(object, x);
    MapObject_SetCurrentX(object, x);

    coords.y = ObjectEvent_GetHeight(objectEvent);
    u32 height = (coords.y >> 3) / FX32_ONE;
    MapObject_SetInitialHeight(object, height);
    MapObject_SetPreviousHeight(object, height);
    MapObject_SetCurrentHeight(object, height);

    u16 y = ObjectEvent_GetYCoord(objectEvent);
    coords.z = y * FX32_CONST(16) + FX32_CONST(8);
    MapObject_SetInitialY(object, y);
    MapObject_SetPreviousY(object, y);
    MapObject_SetCurrentY(object, y);

    MapObject_SetPositionVec(object, &coords);
}

static void sub_02057E90(LocalMapObject *object, MapObjectManager *manager) { //setup facing and flags? also sets manager?
    MapObject_SetFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK12 | MAPOBJECTFLAG_UNK11 | MAPOBJECTFLAG_ACTIVE));
    if (MapObject_ScriptIdIsFFFF(object) == TRUE) {
        MapObject_SetFlag25(object, TRUE);
    }
    MapObject_SetManager(object, manager);
    MapObject_SetFacingDirectionDirect(object, MapObject_GetInitialFacingDirection(object));
    MapObject_SetNextFacingDirection(object, MapObject_GetInitialFacingDirection(object));
    MapObject_ClearHeldMovement(object);
}

/*static*/ void sub_02057EE0(LocalMapObject *object) {
    UnkLMOCallbackStruct *unk = sub_02058D14(MapObject_GetMovement(object));
    sub_02058630(object, sub_02058D2C(unk));
    sub_02058644(object, sub_02058D30(unk));
    sub_02058658(object, sub_02058D34(unk));
}

extern const UnkLMOCallbackStruct2 ov05_021F9080;

/*static*/ void sub_02057F18(LocalMapObject *object) {
    u32 spriteId = MapObject_GetSpriteID(object);
    UnkLMOCallbackStruct2 *unk = (spriteId == SPRITE_CAMERA_FOCUS) ? (UnkLMOCallbackStruct2 *)&ov05_021F9080 : sub_02058D4C(spriteId);

    sub_02058684(object, sub_02058D38(unk));
    sub_02058698(object, sub_02058D3C(unk));
    sub_020586AC(object, sub_02058D40(unk));
    sub_020586C0(object, sub_02058D44(unk));
    sub_020586D4(object, sub_02058D48(unk));
}

static void MapObject_Clear(LocalMapObject *object) {
    memset(object, 0, sizeof(LocalMapObject));
}

static u32 sub_02057F90(LocalMapObject *object, u32 flagId, u32 objectEventCount, ObjectEvent *objectEvents) {
    for (; objectEventCount != 0; objectEventCount--, objectEvents++) {
        if (ObjectEvent_GetID(objectEvents) != MapObject_GetID(object)) {
            continue;
        }

        if (ObjectEvent_ScriptIDIsUnset(objectEvents) == TRUE) {
            u16 flagIdUnset = ObjectEvent_GetFlagID_AssertScriptIDIsUnset(objectEvents);
            if (MapObject_CheckFlag25(object) == TRUE) { //this and the one below cannot be combined as it doesn't match
                if (sub_02058750(object) == flagIdUnset) {
                    return 1;
                }
            } else if (sub_02058450(object) == flagIdUnset) {
                return 2;
            }
        } else if (MapObject_CheckFlag25(object) == TRUE && sub_02058750(object) == flagId) {
            return 2;
        }
    }

    return 0;
}

//TODO: this also gets based on the second param, which is currently labelled as flagId, but is not
//see the note attached to sub_02058450
static LocalMapObject *MapObjectManager_GetFirstObjectWithID(MapObjectManager *manager, u32 objectId, u32 flagId) {
    s32 index = 0;
    LocalMapObject *object;

    while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        if (MapObject_GetID(object) == objectId && sub_02058450(object) == flagId) {
            return object;
        }
    }

    return NULL;
}

LocalMapObject *MapObjectManager_GetFirstActiveObjectByID(MapObjectManager *manager, u32 id) {
    GF_ASSERT(manager != NULL);

    s32 count = MapObjectManager_GetObjectCount(manager);
    LocalMapObject *objects = (LocalMapObject *)MapObjectManager_GetConstObjects(manager);
    do {
        if (MapObject_TestFlagsBits(objects, MAPOBJECTFLAG_ACTIVE) == TRUE &&
            MapObject_CheckFlag25(objects) == FALSE && MapObject_GetID(objects) == id)
        {
            return objects;
        }

        count--;
        objects++;
    } while (count > 0);

    return NULL;
}

LocalMapObject *MapObjectManager_GetFirstActiveObjectWithMovement(MapObjectManager *manager, u32 movement) {
    s32 count = MapObjectManager_GetObjectCount(manager);
    LocalMapObject *objects = (LocalMapObject *)MapObjectManager_GetConstObjects(manager);

    do {
        if (MapObject_TestFlagsBits(objects, MAPOBJECTFLAG_ACTIVE) == TRUE && MapObject_GetMovement(objects) == movement) {
            return objects;
        }

        count--;
        objects++;
    } while (count > 0);

    return NULL;
}

BOOL MapObjectManager_GetNextObjectWithFlagFromIndex(MapObjectManager *manager, LocalMapObject **objectDest, s32 *index, MapObjectFlagBits flag) {
    s32 count = MapObjectManager_GetObjectCount(manager);
    if (*index >= count) {
        return FALSE;
    }

    LocalMapObject *objects = (LocalMapObject *)MapObjectManager_GetConstObjects(manager);
    LocalMapObject *object = objects + *index;
    do {
        (*index)++;

        if (flag == (MapObjectFlagBits)MapObject_GetFlagsBits(object, flag)) {
            *objectDest = object;
            return TRUE;
        }

        object++;
    } while (*index < count);

    return FALSE;
}

static void sub_02058148(LocalMapObject *object) {
    MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK2);
    sub_0205815C(object);
}

/*static*/ void sub_0205815C(LocalMapObject *object) {
    MapObject_ClearFlagsBits(object, (MapObjectFlagBits)(MAPOBJECTFLAG_UNK26 | MAPOBJECTFLAG_UNK24 |
                                                         MAPOBJECTFLAG_UNK20 | MAPOBJECTFLAG_UNK15));
}

static void MapObject_NoOp(LocalMapObject *object) {
    // NoOp
}

static u32 FieldSystem_ResolveObjectSpriteID(FieldSystem *fieldSystem, s32 spriteId) {
    //BUG: index out of range, should be < not <=
    //TODO: check these with sprite constants, these should be variable sprites but this starts at 101 not 100
    if (spriteId >= 101 && spriteId <= 117) {
        spriteId = FieldSystem_VarGetObjectEventGraphicsId(fieldSystem, (u16)(spriteId - 101));
    }
    return spriteId;
}

/*static*/ void sub_0205818C(LocalMapObject *object) {
    if (MapObject_GetFlagsBits(object, MAPOBJECTFLAG_UNK12) != 0) {
        sub_02059D1C(object);
    }
}

static void sub_020581A4(LocalMapObject *object) {
    sub_02057EE0(object);
    sub_02058EE8(object);
}
