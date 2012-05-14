LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := chipmunk

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include/chipmunk

CHIPMUNK_SRC = src/cpSweep1D.c \
	src/cpSpaceHash.c \
	src/cpSpaceComponent.c \
	src/cpCollision.c \
	src/cpHashSet.c \
	src/cpVect.c \
	src/cpShape.c \
	src/cpBody.c \
	src/cpArray.c \
	src/cpSpatialIndex.c \
	src/constraints/cpDampedRotarySpring.c \
	src/constraints/cpPinJoint.c \
	src/constraints/cpRatchetJoint.c \
	src/constraints/cpRotaryLimitJoint.c \
	src/constraints/cpGearJoint.c \
	src/constraints/cpConstraint.c \
	src/constraints/cpGrooveJoint.c \
	src/constraints/cpSimpleMotor.c \
	src/constraints/cpDampedSpring.c \
	src/constraints/cpSlideJoint.c \
	src/constraints/cpPivotJoint.c \
	src/cpBB.c \
	src/cpBBTree.c \
	src/cpArbiter.c \
	src/cpSpaceStep.c \
	src/chipmunk.c \
	src/cpSpace.c \
	src/cpSpaceQuery.c \
	src/cpPolyShape.c

LOCAL_SRC_FILES := $(CHIPMUNK_SRC)

LOCAL_CFLAGS += -std=c99

include $(BUILD_SHARED_LIBRARY)
