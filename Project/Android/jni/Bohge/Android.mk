LOCAL_PATH := $(call my-dir)

# bohge¿â

include $(CLEAR_VARS)

LOCAL_MODULE := bohge

LOCAL_ARM_MODE := arm

LOCAL_CFLAGS += -DANDROID -DTERRAINMANAGER -DMEMORYPOOL

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_CFLAGS += -D__NEON_MATH__
endif

LOCAL_SRC_FILES := \
	../../../../Librarys/Bohge/Bohge/MemoryPool.c \
	../../../../Librarys/Bohge/Bohge/3DMath.cpp \
	../../../../Librarys/Bohge/Bohge/aabboxDraw.cpp \
	../../../../Librarys/Bohge/Bohge/ActionManager.cpp \
	../../../../Librarys/Bohge/Bohge/AES.cpp \
	../../../../Librarys/Bohge/Bohge/AndroidIOSystem.cpp \
	../../../../Librarys/Bohge/Bohge/AssembledMaterialShader.cpp \
	../../../../Librarys/Bohge/Bohge/AxisNode.cpp \
	../../../../Librarys/Bohge/Bohge/BaseShader.cpp \
	../../../../Librarys/Bohge/Bohge/Bfont.cpp \
	../../../../Librarys/Bohge/Bohge/Billboard.cpp \
	../../../../Librarys/Bohge/Bohge/BloomPS.cpp \
	../../../../Librarys/Bohge/Bohge/Camera.cpp \
	../../../../Librarys/Bohge/Bohge/complex.cpp \
	../../../../Librarys/Bohge/Bohge/ConcisePEShaders.cpp \
	../../../../Librarys/Bohge/Bohge/CopyCurrentScreen.cpp \
	../../../../Librarys/Bohge/Bohge/CubeNode.cpp \
	../../../../Librarys/Bohge/Bohge/Decoder.cpp \
	../../../../Librarys/Bohge/Bohge/DecoderManager.cpp \
	../../../../Librarys/Bohge/Bohge/DOFPostEffect.cpp \
	../../../../Librarys/Bohge/Bohge/Emitter.cpp \
	../../../../Librarys/Bohge/Bohge/EncryptFile.cpp \
	../../../../Librarys/Bohge/Bohge/Engine.cpp \
	../../../../Librarys/Bohge/Bohge/Environment.cpp \
	../../../../Librarys/Bohge/Bohge/EnvironmentShaders.cpp \
	../../../../Librarys/Bohge/Bohge/FakeLights.cpp \
	../../../../Librarys/Bohge/Bohge/fft.cpp \
	../../../../Librarys/Bohge/Bohge/FXAAPostEffect.cpp \
	../../../../Librarys/Bohge/Bohge/GameSetting.cpp \
	../../../../Librarys/Bohge/Bohge/GridNode.cpp \
	../../../../Librarys/Bohge/Bohge/HDRPostEffect.cpp \
	../../../../Librarys/Bohge/Bohge/HDRShaders.cpp \
	../../../../Librarys/Bohge/Bohge/IFile.cpp \
	../../../../Librarys/Bohge/Bohge/IOSystem.cpp \
	../../../../Librarys/Bohge/Bohge/IOSystemBase.cpp \
	../../../../Librarys/Bohge/Bohge/IndicesBuffer.cpp \
	../../../../Librarys/Bohge/Bohge/IPostEffect.cpp \
	../../../../Librarys/Bohge/Bohge/JobBaseThread.cpp \
	../../../../Librarys/Bohge/Bohge/JobManager.cpp \
	../../../../Librarys/Bohge/Bohge/Language.cpp \
	../../../../Librarys/Bohge/Bohge/LessThread.cpp \
	../../../../Librarys/Bohge/Bohge/Light.cpp \
	../../../../Librarys/Bohge/Bohge/LightMaped.cpp \
	../../../../Librarys/Bohge/Bohge/LocatableObject.cpp \
	../../../../Librarys/Bohge/Bohge/Log.cpp \
	../../../../Librarys/Bohge/Bohge/Material.cpp \
	../../../../Librarys/Bohge/Bohge/MaterialCompiler.cpp \
	../../../../Librarys/Bohge/Bohge/MaterialManager.cpp \
	../../../../Librarys/Bohge/Bohge/Model.cpp \
	../../../../Librarys/Bohge/Bohge/ModelData.cpp \
	../../../../Librarys/Bohge/Bohge/ModelShaders.cpp \
	../../../../Librarys/Bohge/Bohge/ModelSkeleton.cpp \
	../../../../Librarys/Bohge/Bohge/MPTDecoder.cpp \
	../../../../Librarys/Bohge/Bohge/NeonMath.cpp \
	../../../../Librarys/Bohge/Bohge/NormalGenerate.cpp \
	../../../../Librarys/Bohge/Bohge/ObjectFactory.cpp \
	../../../../Librarys/Bohge/Bohge/ObjectGroup.cpp \
	../../../../Librarys/Bohge/Bohge/ObjectManager.cpp \
	../../../../Librarys/Bohge/Bohge/ObjectRoot.cpp \
	../../../../Librarys/Bohge/Bohge/ObjectTile.cpp \
	../../../../Librarys/Bohge/Bohge/OCTree.cpp \
	../../../../Librarys/Bohge/Bohge/OGGDecoder.cpp \
	../../../../Librarys/Bohge/Bohge/OpenGLES.cpp \
	../../../../Librarys/Bohge/Bohge/parameterhandler.cpp \
	../../../../Librarys/Bohge/Bohge/Particle.cpp \
	../../../../Librarys/Bohge/Bohge/ParticleShaders.cpp \
	../../../../Librarys/Bohge/Bohge/PhysicManager.cpp \
	../../../../Librarys/Bohge/Bohge/Pipeline.cpp \
	../../../../Librarys/Bohge/Bohge/PlantChunk.cpp \
	../../../../Librarys/Bohge/Bohge/PlantManager.cpp \
	../../../../Librarys/Bohge/Bohge/PlantShaders.cpp \
	../../../../Librarys/Bohge/Bohge/PlantTile.cpp \
	../../../../Librarys/Bohge/Bohge/PrePixelLightShaders.cpp \
	../../../../Librarys/Bohge/Bohge/RealLights.cpp \
	../../../../Librarys/Bohge/Bohge/RendBuffer.cpp \
	../../../../Librarys/Bohge/Bohge/RenderTarget.cpp \
	../../../../Librarys/Bohge/Bohge/ResLoader.cpp \
	../../../../Librarys/Bohge/Bohge/ResourceManager.cpp \
	../../../../Librarys/Bohge/Bohge/RotatingRing.cpp \
	../../../../Librarys/Bohge/Bohge/SceneMain.cpp \
	../../../../Librarys/Bohge/Bohge/SceneManager.cpp \
	../../../../Librarys/Bohge/Bohge/ScriptBase.cpp \
	../../../../Librarys/Bohge/Bohge/ScriptEmitter.cpp \
	../../../../Librarys/Bohge/Bohge/ScriptManager.cpp \
	../../../../Librarys/Bohge/Bohge/Serializer.cpp \
	../../../../Librarys/Bohge/Bohge/ShaderAssembler.cpp \
	../../../../Librarys/Bohge/Bohge/ShaderFile.cpp \
	../../../../Librarys/Bohge/Bohge/ShaderLibrary.cpp \
	../../../../Librarys/Bohge/Bohge/ShaderManager.cpp \
	../../../../Librarys/Bohge/Bohge/ShaderParameter.cpp \
	../../../../Librarys/Bohge/Bohge/Shaders2D.cpp \
	../../../../Librarys/Bohge/Bohge/ShaderSegment.cpp \
	../../../../Librarys/Bohge/Bohge/ShadowCaster.cpp \
	../../../../Librarys/Bohge/Bohge/ShadowShader.cpp \
	../../../../Librarys/Bohge/Bohge/Shapes.cpp \
	../../../../Librarys/Bohge/Bohge/SkyBox.cpp \
	../../../../Librarys/Bohge/Bohge/SoundManager.cpp \
	../../../../Librarys/Bohge/Bohge/SoundManagerSL.cpp \
	../../../../Librarys/Bohge/Bohge/SoundPlayer.cpp \
	../../../../Librarys/Bohge/Bohge/SoundPlayerSL.cpp \
	../../../../Librarys/Bohge/Bohge/SoundResource.cpp \
	../../../../Librarys/Bohge/Bohge/SSAOPostEffect.cpp \
	../../../../Librarys/Bohge/Bohge/TerrainChunk.cpp \
	../../../../Librarys/Bohge/Bohge/TerrainManager.cpp \
	../../../../Librarys/Bohge/Bohge/TerrainShaders.cpp \
	../../../../Librarys/Bohge/Bohge/TerrainTile.cpp \
	../../../../Librarys/Bohge/Bohge/Texture.cpp \
	../../../../Librarys/Bohge/Bohge/TextureState.cpp \
	../../../../Librarys/Bohge/Bohge/ThreadCondition.cpp \
	../../../../Librarys/Bohge/Bohge/Threading.cpp \
	../../../../Librarys/Bohge/Bohge/ThreadMutex.cpp \
	../../../../Librarys/Bohge/Bohge/ThreadRWLock.cpp \
	../../../../Librarys/Bohge/Bohge/UI.cpp \
	../../../../Librarys/Bohge/Bohge/UIAnimation.cpp \
	../../../../Librarys/Bohge/Bohge/UIManager.cpp \
	../../../../Librarys/Bohge/Bohge/UsualFile.cpp \
	../../../../Librarys/Bohge/Bohge/UtilityShaders.cpp \
	../../../../Librarys/Bohge/Bohge/Vertex.cpp \
	../../../../Librarys/Bohge/Bohge/VertexBuffer.cpp \
	../../../../Librarys/Bohge/Bohge/VolumeLightPS.cpp \
	../../../../Librarys/Bohge/Bohge/VolumeLightShaders.cpp \
	../../../../Librarys/Bohge/Bohge/WaterGrid.cpp \
	../../../../Librarys/Bohge/Bohge/WaterManager.cpp \
	../../../../Librarys/Bohge/Bohge/WaterShaders.cpp \
	../../../../Librarys/Bohge/Bohge/WaterTile.cpp \
	../../../../Librarys/Bohge/Bohge/WAVDecoder.cpp \
	../../../../Librarys/Bohge/Bohge/ZipFile.cpp


LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../Librarys/Bohge/Bohge \
$(LOCAL_PATH)/../../../../Librarys/Bohge/BohgeNet/BohgeNet \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/freetype/include \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libpng-1.5.10 \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libzip-0.08.1 \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/nedmalloc_v1.06beta2 \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/lua-5.2.2/Lua \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/bullet/src \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libogg-1.3.0/include \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/libvorbis-1.3.3/include \
$(LOCAL_PATH)/../../../../Librarys/Bohge/External/glsl_optimizer/src

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_C_INCLUDES += $(ANDROID_NDK_ROOT)/sources/cpufeatures
endif

LOCAL_LDLIBS := -lstdc++  -ldl -lc -lGLESv2 -lz -llog -lOpenSLES -lpthread
LOCAL_STATIC_LIBRARIES := zip freetype png BohgeNet lua bullet nedmalloc NeonMath cpufeatures glsloptimizer libvorbis libogg
  
include $(BUILD_STATIC_LIBRARY)

include $(ANDROID_NDK_ROOT)/sources/cpufeatures/Android.mk