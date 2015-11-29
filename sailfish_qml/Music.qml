// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0
import QtMultimedia 5.0

Item{
    id:music

    property string soundPath: "file:///usr/share/harbour-aircraftwar/data/"
    visible: false

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

    Timer{
       interval: 1000

       onTriggered: mysettings.setValue("absoluteVolume", mymixer.absoluteVolume)
    }

    SoundEffect{
        id: game_music

        source: soundPath+"game_music.wav"
        loops: SoundEffect.Infinite
    }

    SoundEffect{
        id: button

        source: soundPath+"button.wav"
    }
    SoundEffect{
        id: achievement


        source: soundPath+"achievement.wav"

    }
    SoundEffect{
        id: flying


        source: soundPath+"flying.wav"

    }
    SoundEffect{
        id: bullet

        source: soundPath+"bullet.wav"
    }
    SoundEffect{
        id: bomb

        source: soundPath+"bomb.wav"
    }
    SoundEffect{
        id: double_laser

        source: soundPath+"double_laser.wav"
    }
    SoundEffect{
        id: enemy1_down

        source: soundPath+"enemy1_down.wav"
    }
    SoundEffect{
        id: enemy2_down

        source: soundPath+"enemy2_down.wav"
    }
    SoundEffect{
        id: enemy3_down

        source: soundPath+"enemy3_down.wav"
    }
    SoundEffect{
        id: game_over

        source: soundPath+"game_over.wav"
    }
    SoundEffect{
        id: out_porp

        source: soundPath+"out_porp.wav"
    }
    SoundEffect{
        id: use_bomb

        source: soundPath+"use_bomb.wav"
    }
}
