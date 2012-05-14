LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_IMG_PATH := ../SDL_image
SDL_TTF_PATH := ../SDL_ttf
CHIPMUNK_PATH := ../chipmunk/include/chipmunk

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/$(SDL_IMG_PATH) \
	$(LOCAL_PATH)/$(SDL_TTF_PATH) \
    $(LOCAL_PATH)/$(CHIPMUNK_PATH)

# Add your application source files here...

LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.cpp \
	main.c \
	renderer.c \
	input.c

LOCAL_SHARED_LIBRARIES := SDL SDL_image SDL2_ttf chipmunk

LOCAL_LDLIBS := -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
