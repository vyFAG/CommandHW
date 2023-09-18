#pragma once

#include <utility>
#include <any>
#include <map>
#include <string>
#include <cmath>
#include <exception>
#include <queue>

using Vector = std::pair<int, int>;



class BaseException : public std::exception
{
public:
    explicit BaseException(const char* message)
        : exceptionMsg(message) {}

    explicit BaseException(const std::string& message)
        : exceptionMsg(message) {}

    virtual ~BaseException() noexcept {}

    virtual const char* what() const noexcept 
    {
        return exceptionMsg.c_str();
    }

    virtual const std::string getMsg() const noexcept 
    {
        return exceptionMsg;
    }

protected:
    std::string exceptionMsg;
};



class RunOutOfFuelException : public BaseException
{
public:
    RunOutOfFuelException(const char* message)
        : BaseException(message) {}

    RunOutOfFuelException(const std::string& message)
        : BaseException(message) {}

    virtual ~RunOutOfFuelException() noexcept {}
};



class ICommand
{
public:
    ICommand(std::string setCommandType)
    {
        commandType = setCommandType;
    }

    virtual void execute() = 0;

    std::string getCommandType()
    {
        return commandType;
    }

protected:
    std::string commandType = "";
};



class UObject
{
public:
    virtual std::any getProperty(std::string key) = 0;
    virtual void     setProperty(std::string key, std::any newValue) = 0;
};



class Movable
{
    virtual Vector getPosition() = 0;
    virtual Vector getVelocity() = 0;
    virtual void setPosition(Vector newV) = 0;
};



class MovableAdp : public Movable
{
public:
    MovableAdp() {};
    MovableAdp(UObject* setObj);

    virtual Vector getPosition();
    virtual Vector getVelocity();
    virtual void setPosition(Vector newV);

private:
    UObject* movableObj = nullptr;
};



class MoveCommand : public ICommand
{
public:
    MoveCommand(MovableAdp setMove);
    void execute();

private:
    MovableAdp moveObj;
};



class Rotatable
{
public:
    virtual int getDirection() = 0;
    virtual int getAngularVelocity() = 0;
    virtual void setDirection(int newV) = 0;
    virtual int getDirectionsNumber() = 0;
};



class RotatableAdp : public Rotatable
{
public:
    RotatableAdp() {};
    RotatableAdp(UObject* setObj);

    virtual int getDirection();
    virtual int getAngularVelocity();
    virtual void setDirection(int newV);
    virtual int getDirectionsNumber();

private:
    UObject* rotatableObj = nullptr;
};



class RotateCommand : public ICommand
{
public:
    RotateCommand(RotatableAdp setRotate);
    void execute();

private:
    RotatableAdp rotateObj;
};



class Spaceship : public UObject
{
public:
    Spaceship();

    virtual std::any getProperty(std::string key);
    virtual void     setProperty(std::string key, std::any newValue);

private:
    std::map<std::string, std::any> spaceshipData;
};



class CheckFuelCommand : public ICommand
{
public:
    CheckFuelCommand(UObject* setSpaceship);
    void execute();

private:
    UObject* spaceship;
};



class BurnFuelCommand : public ICommand
{
public:
    BurnFuelCommand(UObject* setSpaceship);
    void execute();

private:
    UObject* spaceship;
};



class MoveWithFuel : public ICommand
{
public:
    MoveWithFuel(UObject* setSpaceship);
    void execute();

private:
    UObject* spaceship;
    std::queue<ICommand*> commands = {};
};



class MacroCommand
{
public:
    MacroCommand(std::queue<ICommand*> setCommandList);
    void execute();

private:
    std::queue<ICommand*> commandList = {};
};