project ( WavPlayer )

set( QT_USE_QTCORE true )
set( QT_USE_QTGUI true )

include(${INSTALL_QT_USE_FILE})

include_directories( ${QT_INCLUDE_DIR} )


PROTOBUF_GENERATE_CPP(ProtoSourcesAudioPlayer ProtoHeadersAudioPlayer AudioPlayerIfc.proto)

YACOMPONENT_IFC_GENERATE(GENERATED_IFC_AUDIOPLAYER AudioPlayerIfc.xml )


YACOMPONENT_GENERATE(GENERATED_COMP_WAV_SRC WavFilePlayerComponent.xml)
YACOMPONENT_GENERATE(GENERATED_COMP_SOUND_SRC SoundOutComponent.xml)

YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT WavPlayer.xml)

add_library ( wavplayerlib  ${GENERATED_IFC_AUDIOPLAYER}
                            ${GENERATED_COMP_WAV_SRC}
                            ${GENERATED_COMP_SOUND_SRC}
                            ${GENERATED_LAYOUT}
                            ${ProtoSourcesAudioPlayer}
                            ${ProtoHeadersAudioPlayer}
)
