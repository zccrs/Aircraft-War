// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
import GEAudio 1.0

Item{
    id:music
    visible: false
    property string soundPath: utility.productName=="pc"?':/sound/':'./sound/'

    function start_music(string)
    {
        switch(string)
        {
        case "game_music":
            if(mysettings.getValue("game_music_switch","true")=="true")
                game_music.play()
            break;
        case "button":
            if(mysettings.getValue("sound_switch","true")=="true")
                button.play()
            break;
        case "achievement":
            if(mysettings.getValue("sound_switch","true")=="true")
                achievement.play()
            break
        case "flying":
            if(mysettings.getValue("sound_switch","true")=="true")
                flying.play()
            break;
        case "bullet":
            if(mysettings.getValue("sound_switch","true")=="true")
                bullet.play()
            break;
        case "bomb":
            if(mysettings.getValue("sound_switch","true")=="true")
                bomb.play()
            break;
        case "double_saler":
            if(mysettings.getValue("sound_switch","true")=="true")
                double_laser.play()
            break;
        case "enemy1_down":
            if(mysettings.getValue("sound_switch","true")=="true")
                enemy1_down.play()
            break;
        case "enemy2_down":
            if(mysettings.getValue("sound_switch","true")=="true")
                enemy2_down.play()
            break;
        case "enemy3_down":
            if(mysettings.getValue("sound_switch","true")=="true")
                enemy3_down.play()
            break;
        case "game_over":
            if(mysettings.getValue("sound_switch","true")=="true")
                game_over.play()
            break;
        case "out_porp":
            if(mysettings.getValue("sound_switch","true")=="true")
                out_porp.play()
            break;
        case "use_bomb":
            if(mysettings.getValue("sound_switch","true")=="true")
                use_bomb.play()
            break;
        default:break;

        }
    }
    function stop_music(string)
    {
        switch(string)
        {
        case "game_music":
            game_music.stop()
            break;
        case "button":
            button.stop()
            break;
        case "achievement":
            achievement.stop()
            break
        case "flying":
            flying.stop()
            break;
        case "bullet":
            bullet.stop()
            break;
        case "bomb":
            bomb.stop()
            break;
        case "double_saler":
            double_laser.stop()
            break;
        case "enemy1_down":
            enemy1_down.stop()
            break;
        case "enemy2_down":
            enemy2_down.stop()
            break;
        case "enemy3_down":
            enemy3_down.stop()
            break;
        case "game_over":
            game_over.stop()
            break;
        case "out_porp":
            out_porp.stop()
            break;
        case "use_bomb":
            use_bomb.stop()
            break;
        default:
            break;
        }
    }


    AudioMixer {
        id: mymixer

        enabled: true
        absoluteVolume: 0.5
        onAddVolum: {
            utility.console("加音量")
            if( mymixer.absoluteVolume<1 )
                mymixer.absoluteVolume +=0.1
        }
        onSubVolum: {
            utility.console("减音量")
            if( mymixer.absoluteVolume>0 )
                mymixer.absoluteVolume -=0.1
        }

        effect: [
            EchoEffect {
                id: masterEcho
                delay: 0.001
                decay: 0.0
            },
            CutOffEffect {
                id: masterCutoff
                cutOff: 1.0
                resonance: 1.0
            }
        ]

        AudioBuffer {
            id: game_music
            volume: 0.6
            loopCount: 999
            source: soundPath+"game_music.wav"
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }

            ]

        }
        AudioBuffer {
            id: button
            source: soundPath+"button.wav"
            volume: 0.7
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]

        }
        AudioBuffer {
            id: achievement
            source: soundPath+"achievement.wav"
            volume: 0.7
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: flying
            source: soundPath+"flying.wav"
            volume: 0.8
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: bullet
            source: soundPath+"bullet.wav"
            volume: 0.8
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: bomb
            source: soundPath+"bomb.wav"
            volume: 0.9
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: double_laser
            source: soundPath+"double_laser.wav"
            volume: 0.9
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: enemy1_down
            source: soundPath+"enemy1_down.wav"
            volume: 0.8
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: enemy2_down
            source: soundPath+"enemy2_down.wav"
            volume: 0.8
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: enemy3_down
            source: soundPath+"enemy3_down.wav"
            volume: 0.8
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: game_over
            source: soundPath+"game_over.wav"
            volume: 0.9
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: out_porp
            source: soundPath+"out_porp.wav"
            volume: 0.9
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
        AudioBuffer {
            id: use_bomb
            source: soundPath+"use_bomb.wav"
            volume: 0.8
            effect: [
                EchoEffect {
                    delay: 0.001
                    decay: 0.0
                },
                CutOffEffect {
                    cutOff: 1.0
                    resonance: 1.0
                }
            ]
        }
    }
}
