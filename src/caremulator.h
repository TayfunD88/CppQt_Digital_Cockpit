#ifndef CAREMULATOR_H
#define CAREMULATOR_H

#include <QtGui>
#include <QtQml>


class CarEmulator : public QObject
{
private:
    Q_OBJECT

    Q_PROPERTY(bool driveBackwards READ getDriveBackwards  WRITE setDriveBackwards  NOTIFY driveBackwardsChanged);
    Q_PROPERTY(bool engineStarted  READ getEngineStarted   WRITE setEngineStarted   NOTIFY engineStartedChanged);

    Q_PROPERTY(QString displayInfo READ getDisplayInfo     WRITE setDisplayInfo     NOTIFY displayInfoChanged);

    Q_PROPERTY(char driveMode      READ getDriveMode       WRITE setDriveMode       NOTIFY driveModeChanged);
    Q_PROPERTY(char gear            READ getGear            WRITE setGear            NOTIFY gearChanged);

    Q_PROPERTY(int velocity        READ getVelocity        WRITE setVelocity        NOTIFY velocityChanged);
    Q_PROPERTY(int engineSpeed     READ getEngineSpeed     WRITE setEngineSpeed     NOTIFY engineSpeedChanged);

    Q_PROPERTY(int fuelTank        READ getFuelTank        WRITE setFuelTank        NOTIFY fuelTankChanged);
    Q_PROPERTY(int fuelConsumption READ getFuelConsumption WRITE setFuelConsumption NOTIFY fuelConsumptionChanged);
    Q_PROPERTY(int drivenKilometer READ getDrivenKilometer WRITE setDrivenKilometer NOTIFY drivenKilometerChanged);

    bool m_driveBackwards;
    bool m_engineStarted;

    QString m_displayInfo;

    char m_gear;
    char m_driveMode;

    int m_velocity;
    int m_engineSpeed;

    int m_fuelTankMax;
    int m_fuelTank;
    int m_fuelConsumption;
    int m_drivenKilometer;

    char m_gearOld;
    int m_gearRatio;
    int m_idleSpeed;
    int m_minEngineSpeed;
    int m_maxEngineSpeed;
    int m_minVelocity;
    int m_maxVelocity;

public:
    explicit CarEmulator(QObject *parent = nullptr);

    Q_INVOKABLE void listenTo(QObject *object);
    bool eventFilter(QObject *object, QEvent *event) override;

    void engineStart();
    void engineStop();
    void accelerate();
    void brake();
    void cruiseControl();

    void automaticGear(const int velocity);

    void calcDrivenKilometers();
    void calcFuelTankLevel();
    void calcFuelConsumption();
    void tankGas();

    bool getEngineStarted() const;
    void setEngineStarted(bool newEngineStarted);

    int getVelocity() const;
    void setVelocity(int newVelocity);

    int getEngineSpeed() const;
    void setEngineSpeed(int newEngineSpeed);

    int getFuelConsumption() const;
    void setFuelConsumption(int newFuelConsumption);

    int getDrivenKilometer() const;
    void setDrivenKilometer(int newDrivenKilometer);

    bool getDriveBackwards() const;
    void setDriveBackwards(bool newDriveBackwards);

    const QString &getDisplayInfo() const;
    void setDisplayInfo(const QString &newDisplayInfo);

    char getDriveMode() const;
    void setDriveMode(char newDriveMode);

    int getFuelTank() const;
    void setFuelTank(int newFuelTank);

    char getGear() const;
    void setGear(char newGear);

signals:

    void engineStartedChanged();
    void velocityChanged();
    void engineSpeedChanged();
    void fuelConsumptionChanged();
    void drivenKilometerChanged();
    void driveBackwardsChanged();
    void displayInfoChanged();
    void driveModeChanged();
    void fuelTankChanged();
    void gearChanged();

};

[[maybe_unused]]
static QObject *keyListenerInstance(QQmlEngine *, QJSEngine *engine)
{
    return new CarEmulator(engine);
}


#endif // CAREMULATOR_H
