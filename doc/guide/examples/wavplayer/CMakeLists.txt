project ( WavPlayer )

PROTOBUF_GENERATE(OUT_VAR ProtoSourcesAudioPlayer LANGUAGE cpp PROTOS AudioPlayerIfc.proto APPEND_PATH ${CMAKE_CURRENT_SOURCE_DIR})

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_AUDIOPLAYER AudioPlayerIfc.xml )

YACOMPONENT_GENERATE(GENERATED_COMP_WAV_SRC WavFilePlayerComponent.xml)
YACOMPONENT_GENERATE(GENERATED_COMP_SOUND_SRC SoundOutComponent.xml)

YACOMPONENT_LAYOUT_GENERATE(WavPlayer.xml)

add_library ( wavplayerlib  ${GENERATED_IFC_AUDIOPLAYER}
                            ${GENERATED_COMP_WAV_SRC}
                            ${GENERATED_COMP_SOUND_SRC}
                            ${ProtoSourcesAudioPlayer}
                            ${ProtoHeadersAudioPlayer}
)

target_link_libraries(wavplayerlib  PRIVATE YaComponent::YaComponent )
