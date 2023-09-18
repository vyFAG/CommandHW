#include "pch.h"
#include "CppUnitTest.h"
#include "CommandHW.h"
#include "CommandHW.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommandHWTest
{
	TEST_CLASS(CommandHWTest)
	{
	public:
		
		TEST_METHOD(MovingObjEx)
		{
			Spaceship* spaceship = new Spaceship();

			spaceship->setProperty("position", Vector(1, 2));
			spaceship->setProperty("velocity", 1);
			spaceship->setProperty("direction", 0);
			spaceship->setProperty("directionsNumber", 8);

			MovableAdp moveAd(spaceship);
			moveAd.setPosition({ 1, 2 });

			MoveCommand move(moveAd);

			move.execute();
			Vector returnValue = moveAd.getPosition();

			Assert::IsTrue(returnValue == Vector(2, 2));
		}

		TEST_METHOD(RotatingObjEx)
		{
			Spaceship* spaceship = new Spaceship();

			spaceship->setProperty("angularVelocity", 1);
			spaceship->setProperty("direction", 0);

			RotatableAdp rotateAd(spaceship);
			rotateAd.setDirection(0);

			RotateCommand rotate(rotateAd);

			rotate.execute();
			int returnValue = rotateAd.getDirection();

			Assert::IsTrue(returnValue == 1);
		}

		TEST_METHOD(CheckFuelTest)
		{
			Spaceship* spaceship = new Spaceship();

			spaceship->setProperty("fuel", 10.0);
			spaceship->setProperty("fuelConsumption", 10.1);

			CheckFuelCommand command(spaceship);

			BaseException* exception;
			try
			{
				command.execute();
			}
			catch (BaseException* ex)
			{
				exception = ex;
				Assert::IsTrue(true);
				return;
			}
			
			Assert::IsTrue(false);
		}

		TEST_METHOD(BurnFuelTest)
		{
			Spaceship* spaceship = new Spaceship();

			spaceship->setProperty("fuel", 10.0);
			spaceship->setProperty("fuelConsumption", 1.0);

			BurnFuelCommand command(spaceship);

			command.execute();

			Assert::IsTrue(std::any_cast<double>(spaceship->getProperty("fuel")) == 9);
		}

		TEST_METHOD(MoveWithFuelTest)
		{
			Spaceship* spaceship = new Spaceship();

			spaceship->setProperty("position", Vector(1, 2));
			spaceship->setProperty("velocity", 1);
			spaceship->setProperty("direction", 0);
			spaceship->setProperty("directionsNumber", 8);
			spaceship->setProperty("fuel", 10.0);
			spaceship->setProperty("fuelConsumption", 1.0);

			MoveWithFuel move(spaceship);
			move.execute();

			Assert::IsTrue(std::any_cast<double>(spaceship->getProperty("fuel")) == 9);
			Assert::IsTrue(std::any_cast<Vector>(spaceship->getProperty("position")) == Vector(2, 2));
		}

		TEST_METHOD(MacroCommandTest)
		{
			Spaceship* spaceship = new Spaceship();

			spaceship->setProperty("position", Vector(1, 2));
			spaceship->setProperty("velocity", 1);
			spaceship->setProperty("direction", 0);
			spaceship->setProperty("directionsNumber", 8);
			spaceship->setProperty("fuel", 10.0);
			spaceship->setProperty("fuelConsumption", 1.0);

			std::queue<ICommand*> commands = {};

			commands.push(new CheckFuelCommand(spaceship));
			commands.push(new BurnFuelCommand(spaceship));

			MovableAdp movable(spaceship);

			commands.push(new MoveCommand(movable));

			MacroCommand macroCommand(commands);
			macroCommand.execute();

			Assert::IsTrue(std::any_cast<double>(spaceship->getProperty("fuel")) == 9);
			Assert::IsTrue(std::any_cast<Vector>(spaceship->getProperty("position")) == Vector(2, 2));
		}
	};
}
