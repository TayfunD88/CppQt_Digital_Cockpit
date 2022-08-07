/**
 * CONVERT DEGREE VALUE TO RADIANT
 */
function converDegToRad(degree) {
    var factor = Math.PI / 180;
    return degree * factor;
}

/**
 * DRAWING TEXT ALONG ARC
 *   Parsing 6 Input Parameter in total
 *      Context:    Related to the Canvas onPaint Context
 *      str:        The String that should get printed within the Canvas
 *      centerX:    Defines x-Component of the Center-Position
 *      centerY:    Defines y-Component of the Center-Position
 *      radius:     Defines the Distance of the Curve from its center
 *      angle:      Defines the starting angle
 */
function drawTextAlongArc(context, str, centerX, centerY, radius, angle)
{
    context.save();
    context.translate(centerX, centerY);
    context.rotate(angle - converDegToRad(2));

    for (var n = 0; n < str.length; n++)
    {
        if (str[n-1] === "1")
        {
            context.rotate((-1) * (angle) / (str.length*0.99));
        }
        else if (str[n-1] === "M")
        {
            context.rotate((-1) * (angle) / (str.length*0.45));
        }
        else
        {
            context.rotate((-1) * (angle) / (str.length*0.65));
        }

        context.save();
        context.translate(0, radius);

        var char1 = str[n];

        context.fillText(char1, 0, 0);
        context.restore();
    }

    context.restore();

}

/**
 *  ROTATION OF THE VELOCITY NEEDLE
 *  Parsing velocity value in order to determine the current
 *  needle position related to the min(0km/h) and max(280km/h) value
 */
function redNeedleRotation(velocity)
{
    var maxVelocity = 280;
    var percentage = velocity / maxVelocity;

    return percentage * 270;
}

/**
 *  ROTATION OF THE ENGINESPEED NEEDLE
 *  Parsing engine-speed value in order to determine the current
 *  needle position related to the min(0rpm) and max(8000rpm) value
 */
function engineSpeedNeedleRotation(engineSpeed)
{
    var maxEngineSpeed = 8000;
    var percentage = engineSpeed / maxEngineSpeed;

    return percentage * 270;
}

/**
 *  DEFINE STARTING POSITION OF VELOCITY NEEDLE
 *  Parsing velocity value in order to determine the starting position
 */
function redNeedleTail(velocity)
{
    var offset = 135; // at 135 deg => 0 km/h
    var percentage = velocity / 280;

    return percentage * 270 + offset;
}

/**
 *  DEFINE STARTING POSITION OF ENGINE-SPEED NEEDLE
 *  Parsing engine-speed value in order to determine the starting position
 */
function engineSpeedNeedleTail(engineSpeed)
{
    var offset = 135; // at 135 deg => 0 RPM
    var percentage = engineSpeed / 8000;

    return percentage * 270 + offset;
}

