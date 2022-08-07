import QtQuick
import QtQuick.Shapes
import Qt5Compat.GraphicalEffects
import "logic.js" as Logic
import backend

// Used for displaying the velocity

Item {
    id: speedometer
    width: 600
    height: 600
    visible: true
    antialiasing: true

    Image {
        id: ellipse_gray_brackground
        source: "/Ellipse_Gray_Background.png"
        anchors.centerIn: parent
    }

    Text {
        id: zero_digit
        x: 90
        y: 475
        text: qsTr("0")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: 30
    }

    Text {
        id: twohundredeighty_digit
        x: 475
        y: 475
        text: qsTr("280")
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: 30
    }

    Image {
        id: white_needles
        source: "\White_Needles.png"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: DropShadow {
            color: "#000000"
            transparentBorder: false
            horizontalOffset: 1
            verticalOffset: 1
            radius: 2
            source: white_needles
        }
    }

    Image {
        id: ellipse_silver_ring
        source: "/Ellipse_Silver_Ring.png"
        anchors.centerIn: parent
    }

    Canvas {
        id: cpb1
        anchors.centerIn: parent
        width: 500
        height: 500
        property int velocity: CarEmulator.velocity
        onPaint: {
            var ctx = getContext("2d");

            var grd = ctx.createRadialGradient(250, 250, 225, 250, 250, 275);
            grd.addColorStop(0.0, "rgba(255, 255, 255, 0.0)");
            grd.addColorStop(0.5, "rgba(25, 255, 255, 0.5)");
            grd.addColorStop(1.0, "rgba(255, 255, 255, 1.0)");

            ctx.beginPath();
            ctx.lineWidth = 40;
            ctx.strokeStyle = grd;
            ctx.arc(cpb1.width/2, cpb1.height/2, cpb1.width/2-20, Logic.converDegToRad(135), Logic.converDegToRad(Logic.redNeedleTail(CarEmulator.velocity)));
            ctx.stroke();

            var grd2 = ctx.createRadialGradient(250, 250, 175, 250, 250, 225);
            grd2.addColorStop(0.0, "rgba(255, 255, 255, 0.0)");
            grd2.addColorStop(0.5, "rgba(255, 255, 255, 0.5)");
            grd2.addColorStop(1.0, "rgba(255, 255, 255, 1.0)");

            ctx.beginPath();
            ctx.lineWidth = 60;
            ctx.strokeStyle = grd2;
            ctx.arc(cpb1.width/2, cpb1.height/2, cpb1.width/2-80, Logic.converDegToRad(135), Logic.converDegToRad(Logic.redNeedleTail(CarEmulator.velocity)));
            ctx.stroke();
        }

        function clear()
        {
            var ctx = getContext("2d");
            ctx.reset();
            this.requestPaint();
        }

        onVelocityChanged: clear()
    }

    Image {
        id: red_needle
        source: "\Red_Needle.png"
        anchors.centerIn: parent
        rotation: Logic.redNeedleRotation(CarEmulator.velocity) // [0 deg = 0km/h] | [270 deg = 280 km/h]
    }

    Image {
        id: ellipse_red_needle
        source: "\Ellipse_Red_Needle.png"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: DropShadow {
            color: "#000000"
            transparentBorder: false
            horizontalOffset: 4
            verticalOffset: 6
            radius: 16
            source: ellipse_red_needle
        }
    }

    Glow {
        id: glow_ellipse_red_needle
        anchors.fill: ellipse_red_needle
        radius: 8
        color: "red"
        source: ellipse_red_needle
    }

    Text {
        id: speed_unit
        x: 150
        y: 150
        width: 300
        height: 110
        text: qsTr("km/h")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: 30
    }

    Text {
        id: speed_value
        x: 150
        y: 150
        width: 300
        height: 300
        text: CarEmulator.velocity
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "Righteous"
        color: "#ffffff"
        font.pointSize: 120
    }

}
