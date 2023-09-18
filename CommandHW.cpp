#include "CommandHW.h"



// Movable adapter
MovableAdp::MovableAdp(UObject* setObj)
{
    movableObj = setObj;
}



Vector MovableAdp::getPosition()
{
    return std::any_cast<Vector>(movableObj->getProperty("position"));
}



Vector MovableAdp::getVelocity()
{
    int d = std::any_cast<int>(movableObj->getProperty("direction"));
    int n = std::any_cast<int>(movableObj->getProperty("directionsNumber"));
    int v = std::any_cast<int>(movableObj->getProperty("velocity"));

    return Vector(v * cos((double)d / 360 * n), v * sin((double)d / 360 * n));
}



void MovableAdp::setPosition(Vector newV)
{
    movableObj->setProperty("position", newV);
}



// MovingObj class
MoveCommand::MoveCommand(MovableAdp setMove) : ICommand("MoveCommand")
{
    moveObj = setMove;
}



void MoveCommand::execute()
{
    int newPosX = moveObj.getPosition().first +
        moveObj.getVelocity().first;

    int newPosY = moveObj.getPosition().second +
        moveObj.getVelocity().second;

    moveObj.setPosition(Vector(newPosX, newPosY));
}



// Rotatable adapter
RotatableAdp::RotatableAdp(UObject* setObj)
{
    rotatableObj = setObj;
}



int RotatableAdp::getDirection()
{
    return std::any_cast<int>(rotatableObj->getProperty("direction"));
}



int RotatableAdp::getAngularVelocity()
{
    return std::any_cast<int>(rotatableObj->getProperty("angularVelocity"));
}



void RotatableAdp::setDirection(int newV)
{
    rotatableObj->setProperty("direction", newV);
}



int RotatableAdp::getDirectionsNumber()
{
    return std::any_cast<int>(rotatableObj->getProperty("directionsNumber"));
}



// RotatingObj class
RotateCommand::RotateCommand(RotatableAdp setRotate) : ICommand("RotateCommand")
{
    rotateObj = setRotate;
}



void RotateCommand::execute()
{
    int newDirection = rotateObj.getDirection() + rotateObj.getAngularVelocity();

    rotateObj.setDirection(newDirection);
}



Spaceship::Spaceship()
{
    spaceshipData["position"] = 0;
    spaceshipData["velocity"] = 0;
    spaceshipData["direction"] = 0;
    spaceshipData["directionsNumber"] = 8;
    spaceshipData["angularVelocity"] = 1;
    spaceshipData["fuel"] = 0;
    spaceshipData["fuelConsumption"] = 0;
}



std::any Spaceship::getProperty(std::string key)
{
    return spaceshipData[key];
}



void     Spaceship::setProperty(std::string key, std::any newValue)
{
    spaceshipData[key] = newValue;
}



CheckFuelCommand::CheckFuelCommand(UObject* setSpaceship) : ICommand("CheckFuelCommand")
{
    spaceship = setSpaceship;
}



void CheckFuelCommand::execute()
{
    double fuel = std::any_cast<double>(spaceship->getProperty("fuel"));
    double fuelConsumption = std::any_cast<double>(spaceship->getProperty("fuelConsumption"));

    if (fuel < fuelConsumption)
    {
        throw new RunOutOfFuelException("RunOutOfFuelException");
    }
}



BurnFuelCommand::BurnFuelCommand(UObject* setSpaceship) : ICommand("BurnFuelCommand")
{
    spaceship = setSpaceship;
}



void BurnFuelCommand::execute()
{
    double fuel = std::any_cast<double>(spaceship->getProperty("fuel"));
    double fuelConsumption = std::any_cast<double>(spaceship->getProperty("fuelConsumption"));

    spaceship->setProperty("fuel", fuel - fuelConsumption);
}



MoveWithFuel::MoveWithFuel(UObject* setSpaceship) : ICommand("MoveWithFuel")
{
    spaceship = setSpaceship;
}



void MoveWithFuel::execute()
{
    commands.push(new CheckFuelCommand(spaceship));
    commands.push(new BurnFuelCommand(spaceship));

    MovableAdp movable(spaceship);

    commands.push(new MoveCommand(movable));

    MacroCommand macroCommand(commands);
    macroCommand.execute();
}



MacroCommand::MacroCommand(std::queue<ICommand*> setCommandList)
{
    commandList = setCommandList;
}



void MacroCommand::execute()
{
    while (commandList.size() > 0)
    {
        try
        {
            commandList.front()->execute();
        }
        catch (BaseException* ex)
        {
            break;
        }

        commandList.pop();
    }
}