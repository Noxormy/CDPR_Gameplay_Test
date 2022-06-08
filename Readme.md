# How to run this project

1. Clone this repository with git.
2. Make sure that submodules are downloaded. If not, open repository folder with terminal and run command ```git submodule update --init --recursive```
2. Open downloaded folder
3. Run **install.bat**
4. Open solution with Visual Studio / Rider
5. Choose **Resease|x64 / Debug|x64** configuration
6. Run

## Algorithm

You can learn more about the algorithm [here](https://github.com/Noxormy/CDPR_Gameplay_Test/blob/master/Flocking%20Simulation.pdf)

# FAQ
Q: I get an error that there are no **CMake.txt** files.  
A: It looks like the submodules are not loaded, you need to open the repository directory with git or a terminal and run ``git submodule update --init --recursive``.

Q: I'm getting an error that cmake can't find the **Visual Studio 15 2017** tools.  
A: This project is developed for the Visual Studio 2017 toolkit, you can either install Visual Studio 2017 or update **install.bat** to use your toolkit, for example: **Visual Studio 17 2022** if you have Visual Studio 2022.

Q: When I run the install.bat, I get an error "doesn't match the generator used previously"  
A: You need to open **packages/reactphysics3d** and remove **CMakeFiles** and **CMakeCache.txt**.

Q: I cannot run the application. I get an error that I cannot find the **Windows SDK 10.0.17763.0**.  
A: This project was developed for that version of the Windows SDK. You can install this sdk either through Visual Studio or through [official website](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive/).

Q: I don't want to build project by myself.  
A: Okay, then you can download prebuilt project [here](https://drive.google.com/file/d/1MJcHSlDSvTq1qlDGwvE0Z4VXznJ-3k-4/).
