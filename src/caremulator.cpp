#include "caremulator.h"


CarEmulator::CarEmulator(QObject *parent):
    QObject{parent}
  ,m_driveBackwards{false}
  ,m_engineStarted{false}
  ,m_displayInfo{"Welcome"}
  ,m_gear{' '}
  ,m_driveMode{'P'}
  ,m_velocity{0}
  ,m_engineSpeed{0}
  ,m_fuelTankMax{10000}
  ,m_fuelTank{m_fuelTankMax}
  ,m_fuelConsumption{0}
  ,m_drivenKilometer{0}
  ,m_gearOld{' '}
  ,m_gearRatio{10}
  ,m_idleSpeed{800}
  ,m_minEngineSpeed{0}
  ,m_maxEngineSpeed{6000}
  ,m_minVelocity{0}
  ,m_maxVelocity{280}
{

}

/***
 * Will be handed over in main.qml as mainWindow_1
*/
void CarEmulator::listenTo(QObject *object)
{
    if (!object){
        return;
    }
    object->installEventFilter(this);
}

/***
 * As soon as the upper listenTo-function gets an object it is forwarded to the
 * event filter.
 * Through filtering by events the function recognizes the pressed key and executes
 * code accordingly.
 *
 * From Qt-Documentation:
 * When the filter object's eventFilter() implementation is called,
 * it can accept or reject the event, and allow or deny further processing
 * of the event. If all the event filters allow further processing of an event
 * (by each returning false), the event is sent to the target object itself.
 * If one of them stops processing (by returning true), the target and any later
 * event filters do not get to see the event at all.
 */
bool CarEmulator::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "key" << keyEvent->key() << "pressed on" << object;

        if(keyEvent->key() == Qt::Key_S){
            qDebug() << "KeyPressEvent S";
            if(m_engineStarted == false){
                engineStart();
            }
            else {
                engineStop();
            }
        }
        else if(keyEvent->key() == Qt::Key_P){
            qDebug() << "KeyPressEvent P";
            setDriveMode('P');
        }
        else if(keyEvent->key() == Qt::Key_R){
            qDebug() << "KeyPressEvent R";
            setDriveMode('R');
        }
        else if(keyEvent->key() == Qt::Key_N){
            qDebug() << "KeyPressEvent N";
            setDriveMode('N');
        }
        else if(keyEvent->key() == Qt::Key_D){
            qDebug() << "KeyPressEvent D";
            setDriveMode('D');
        }
        else if(keyEvent->key() == Qt::Key_Space){
            qDebug() << "KeyPressEvent Space";
            if (m_fuelTank > 0) {
                accelerate();
                calcDrivenKilometers();
                calcFuelConsumption();
            }
            else {
                setEngineSpeed(m_minEngineSpeed);
                setVelocity(m_minVelocity);
                setDriveMode('P');
                engineStop();
            }
        }
        else if(keyEvent->key() == Qt::Key_Down){
            qDebug() << "KeyPressEvent Down";
            brake();
            calcDrivenKilometers();
            setFuelConsumption(0);
        }
        else if(keyEvent->key() == Qt::Key_F){
            qDebug() << "KeyPressEvent F";
            tankGas();
        }
        else if(keyEvent->key() == Qt::Key_T){
            qDebug() << "KeyPressEvent T";
            if (m_driveMode == 'D' && m_velocity > m_minVelocity) {
                if (m_fuelTank == 0) {
                    setEngineSpeed(m_minEngineSpeed);
                    setVelocity(m_minVelocity);
                    setDriveMode('P');
                    engineStop();
                }
                else {
                    cruiseControl();
                }
            }
        }
        return true;
    }
    return false;
}

void CarEmulator::engineStart()
{
    if (m_driveMode == 'P'){
        setDisplayInfo("Engine Started");
        setEngineStarted(true);
        setFuelConsumption(32);
        setEngineSpeed(m_idleSpeed);
    }
    else {
        setDisplayInfo("Change drive mode\n to [P]");
    }
}

void CarEmulator::engineStop()
{
    if (m_driveMode == 'P'){
        setDisplayInfo("Engine Stopped");
        setEngineStarted(false);
        setFuelConsumption(0);
        setEngineSpeed(m_minEngineSpeed);
    }
    else {
        setDisplayInfo("Change drive mode\n to [P]");
    }
}

/***
 * This function increases the driven kilometers.
 * To realize a better simulation the function increases the drivenKilometer
 * depending on the velocity-range.
 *
*/
void CarEmulator::calcDrivenKilometers()
{
    if (m_driveMode == 'D') {
        if (m_velocity > m_minVelocity && m_velocity <= 40) {
            setDrivenKilometer(m_drivenKilometer += 5);
        }
        else if (m_velocity > 40 && m_velocity <= 80) {
            setDrivenKilometer(m_drivenKilometer += 15);
        }
        else if (m_velocity > 80 && m_velocity <= 120) {
            setDrivenKilometer(m_drivenKilometer += 25);
        }
        else if (m_velocity > 120 && m_velocity <= 160) {
            setDrivenKilometer(m_drivenKilometer += 35);
        }
        else if (m_velocity > 160 && m_velocity <= 200) {
            setDrivenKilometer(m_drivenKilometer += 50);
        }
        else if (m_velocity > 200 && m_velocity <= 240) {
            setDrivenKilometer(m_drivenKilometer += 60);
        }
        else if (m_velocity > 240 && m_velocity <= m_maxVelocity) {
            setDrivenKilometer(m_drivenKilometer += 70);
        }

        emit drivenKilometerChanged();
    }
}

/***
 * This function sets the current fuel consumption and decreases the fuel level.
 * To realize a better simulation the function increases the current fuel consumption
 * and decreases the fuel level depending on the velocity-range.
 *
*/
void CarEmulator::calcFuelConsumption()
{
    if (m_driveMode != 'P') {

        if (m_velocity == m_minVelocity) {
            setFuelConsumption(32);
        }
        else if (m_velocity > m_minVelocity && m_velocity <= 40) {
            setFuelConsumption(50);
            setFuelTank(m_fuelTank -= 2);
        }
        else if (m_velocity > 40 && m_velocity <= 80) {
            setFuelConsumption(70);
            setFuelTank(m_fuelTank -= 4);
        }
        else if (m_velocity > 80 && m_velocity <= 120) {
            setFuelConsumption(110);
            setFuelTank(m_fuelTank -= 6);
        }
        else if (m_velocity > 120 && m_velocity <= 160) {
            setFuelConsumption(150);
            setFuelTank(m_fuelTank -= 8);
        }
        else if (m_velocity > 160 && m_velocity <= 200) {
            setFuelConsumption(200);
            setFuelTank(m_fuelTank -= 10);
        }
        else if (m_velocity > 200 && m_velocity <= 240) {
            setFuelConsumption(280);
            setFuelTank(m_fuelTank -= 12);
        }
        else if (m_velocity > 240 && m_velocity <= m_maxVelocity) {
            setFuelConsumption(360);
            setFuelTank(m_fuelTank -= 14);
        }
        if (m_fuelTank < 2000) {
            setDisplayInfo("Tank almost empty.\n Find next gasoline station!");
        }
        if (m_fuelTank < 0) {
            setFuelTank(0);
        }

        qDebug() << "Tank: " << m_fuelTank;
        emit fuelTankChanged();
        emit fuelConsumptionChanged();
    }
}

/***
 * This function allows to refill the tank. It is invoked by the key-Event "F".
*/
void CarEmulator::tankGas()
{
    if (m_fuelTank < m_fuelTankMax && m_velocity == m_minVelocity) {
        setFuelTank(m_fuelTank + 100);
        setDisplayInfo("Filling tank with gasoline");
    }
    else if (m_fuelTank >= m_fuelTankMax) {
        setFuelTank(m_fuelTankMax);
        setDisplayInfo("Tank is full. Enjoy ride!");
    }
    else {
        setDisplayInfo("Please stand for filling\n up the tank");
    }

    emit fuelTankChanged();
}

/***
 * This function sets the gear depending on the velocity. Depending on the
 * selected gear a different gearRatio is set to simulate a changing increase of
 * the engineSpeed.
*/
void CarEmulator::automaticGear(const int velocity)
{
    m_gearOld = m_gear;

    if(m_driveMode == 'D'){
        if (velocity <= 25) {
            setGear('1');
            m_gearRatio = 10;
        }
        if (velocity >= 25 && velocity <= 34) {
            setGear('2');
            m_gearRatio = 9;
        }
        else if (velocity >= 35 && velocity <= 49) {
            setGear('3');
            m_gearRatio = 7;
        }
        else if (velocity >= 50 && velocity <= 79) {
            setGear('4');
            m_gearRatio = 6;
        }
        else if (velocity >= 80 && velocity <= 99) {
            setGear('5');
            m_gearRatio = 4;
        }
        else if (velocity >= 100) {
            setGear('6');
            m_gearRatio = 2;
        }
        if (m_gearOld != m_gear) {
            if (m_gear != ' ' && static_cast<int>(m_gear) > static_cast<int>(m_gearOld)) {
                setEngineSpeed(m_engineSpeed - 1000);
            }
        }
    }
    else{
        setGear(' ');
    }
}

/***
 * Depending on the selected driveMode this function increases the velocity and
 * engineSpeed. In case of driveMode 'D' the engineSpeed is increased depending
 * on the gear.
*/
void CarEmulator::accelerate()
{
    switch (m_driveMode) {
    case 'P':
        if (!m_engineStarted)
            setDisplayInfo("Start the engine");
        else
            setDisplayInfo("Change drive mode to [D]");
        break;
    case 'R':
        setGear(' ');
        if (m_velocity < 30) {
            setEngineSpeed(m_engineSpeed + 20);
            setVelocity(m_velocity + 1);
        }
        break;
    case 'N':
        if (m_engineSpeed < m_maxEngineSpeed) {
            setEngineSpeed(m_engineSpeed + 20);
        }
        break;
    case 'D':
        if (m_velocity < m_maxVelocity) {
            setEngineSpeed(m_engineSpeed + 10 * m_gearRatio);
            setVelocity(m_velocity + 1);
            automaticGear(m_velocity);
        }
        break;
    }
    qDebug() << "engineSpeed: " << m_engineSpeed;
}

/***
 * If invoked by the keyEvent 'Key_Down' this function decreases the velocity
 * and engineSpeed.
*/
void CarEmulator::brake()
{
    if (m_velocity > 3) {
        setVelocity(m_velocity -= 3);
        automaticGear(m_velocity);
    }
    else {
        setVelocity(m_minVelocity);
    }

    if (m_engineSpeed > m_idleSpeed) {
        setEngineSpeed(m_engineSpeed -= 80);
    }
    else if (m_engineSpeed <= m_idleSpeed) {
        setEngineSpeed(m_idleSpeed);
    }

    emit engineSpeedChanged();
    emit velocityChanged();
}

/***
 * If invoked by the keyEvent 'Key_T' the cruiseControl-function keeps increasing the
 * drivenKilometers and emptying the fuelTank depending on the current velocity.
*/
void CarEmulator::cruiseControl()
{
    setDisplayInfo("Cruise control active");
    calcFuelConsumption();
    calcDrivenKilometers();
}

/***
 * Depending on the keyEvent this function checks if the simulation is in a
 * valid state to change the DriverMode. If it is allowed, a new driveMode is selected,
 * else a warning is displayed.
*/
void CarEmulator::setDriveMode(char newDriveMode)
{
    if (m_driveMode == newDriveMode)
        return;

    if(newDriveMode == 'P' || newDriveMode == 'R' || newDriveMode == 'N' || newDriveMode == 'D'){
        if (m_engineStarted) {
            if (newDriveMode == 'D'){
                setGear('1');
                setEngineSpeed(m_idleSpeed);
            }

            if(m_velocity > m_minVelocity){
                if(newDriveMode == 'P'){
                    setDisplayInfo("Please switch to [P]\n only in stand");
                    return;
                }
                else if(newDriveMode == 'R'){
                    setDisplayInfo("Please switch to [R]\n only in stand");
                    return;
                }
                else if(newDriveMode == 'N'){
                    setDisplayInfo("Please switch to [N]\n only in stand");
                    return;
                }
            }
            else {
                if (newDriveMode == 'N') {
                    setGear(' ');
                    setFuelConsumption(32);
                }
                if (newDriveMode == 'P') {
                    setGear(' ');
                    setFuelConsumption(32);
                }
            }

            if(newDriveMode == 'R'){
                setDriveBackwards(true);
            }
            else{
                setDriveBackwards(false);
            }

            setDisplayInfo("");
            m_driveMode = newDriveMode;
            emit driveModeChanged();
        }
        else {
            setDisplayInfo("Start the engine");
        }
    }
}

/***
 * In the following, depending on the QProperty Objects the getters and setters
 * are listed.
*/
bool CarEmulator::getEngineStarted() const
{
    return m_engineStarted;
}
void CarEmulator::setEngineStarted(bool newEngineStarted)
{
    if (m_engineStarted == newEngineStarted)
        return;
    m_engineStarted = newEngineStarted;
    emit engineStartedChanged();
}

int CarEmulator::getVelocity() const
{
    return m_velocity;
}
void CarEmulator::setVelocity(int newVelocity)
{
    if (m_velocity == newVelocity)
        return;
    m_velocity = newVelocity;
    emit velocityChanged();
}

int CarEmulator::getEngineSpeed() const
{
    return m_engineSpeed;
}
void CarEmulator::setEngineSpeed(int newEngineSpeed)
{
    if (m_engineSpeed == newEngineSpeed)
        return;
    m_engineSpeed = newEngineSpeed;
    emit engineSpeedChanged();
}

int CarEmulator::getFuelConsumption() const
{
    return m_fuelConsumption;
}
void CarEmulator::setFuelConsumption(int newFuelConsumption)
{
    if (m_fuelConsumption == newFuelConsumption)
        return;
    m_fuelConsumption = newFuelConsumption;
    emit fuelConsumptionChanged();
}

int CarEmulator::getDrivenKilometer() const
{
    return m_drivenKilometer;
}
void CarEmulator::setDrivenKilometer(int newDrivenKilometer)
{
    if (m_drivenKilometer == newDrivenKilometer)
        return;
    m_drivenKilometer = newDrivenKilometer;
    emit drivenKilometerChanged();
}

bool CarEmulator::getDriveBackwards() const
{
    return m_driveBackwards;
}
void CarEmulator::setDriveBackwards(bool newDriveBackwards)
{
    if (m_driveBackwards == newDriveBackwards)
        return;
    m_driveBackwards = newDriveBackwards;
    emit driveBackwardsChanged();
}

const QString &CarEmulator::getDisplayInfo() const
{
    return m_displayInfo;
}
void CarEmulator::setDisplayInfo(const QString &newDisplayInfo)
{
    if (m_displayInfo == newDisplayInfo)
        return;
    m_displayInfo = newDisplayInfo;
    emit displayInfoChanged();
}

char CarEmulator::getDriveMode() const
{
    return m_driveMode;
}

int CarEmulator::getFuelTank() const
{
    return m_fuelTank;
}
void CarEmulator::setFuelTank(int newFuelTank)
{
    if (m_fuelTank == newFuelTank)
        return;
    m_fuelTank = newFuelTank;
    emit fuelTankChanged();
}

char CarEmulator::getGear() const
{
    return m_gear;
}
void CarEmulator::setGear(char newGear)
{
    if (m_gear == newGear)
        return;
    m_gear = newGear;
    emit gearChanged();
}
