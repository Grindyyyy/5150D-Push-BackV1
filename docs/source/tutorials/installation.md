# 1 - Installation

## Install Visual Studio Code
dLib was fully created and is supported by [Visual Studio Code](https://code.visualstudio.com/). Due to this, we recommend using it as an editor.

[Learn more about setting up VSC](https://code.visualstudio.com/docs/setup/setup-overview)

## Install PROS IDE
dLib is a [PROS](https://pros.cs.purdue.edu/) library that can be installed as an [extension](https://marketplace.visualstudio.com/items?itemName=sigbots.pros).

1. Click on the extensions icon (the icon with the blocks)
2. Search up ```PROS``` in the search box
3. Click ```Install``` on the PROS extension created by Purdue ACM Sigbots
4. After PROS is installed, you'll be prompted to install the PROS toolchain in the bottom right corner of your screen
5. Once the toolchain is installed, you will see the PROS icon below the extensions icon, click on this icon
6. Click ```Create Project``` on the menu that appears after clicking the PROS icon and select an empty/new folder
7. Give the project a name, select v5, and select the latest kernel version
8. A new folder will open that contains your PROS project

Congrats! You have successfully installed PROS.

## Install dLib
In order to use dLib, you must download the template file.

1. Download the [dLib template file](https://drive.google.com/file/d/1pZTnNNAe9BUsjXV5kMuxHHEJoFYmJ0i8/view?usp=sharing)
2. Move this template file into your project directory (on the top level)
3. Click on the PROS icon located below the extensions icon as mentioned earlier
4. Click ```Integrated Terminal``` on the menu that appears after clicking the PROS icon
5. Inside of this terminal, type ```pros c fetch dlib@0.0.2.zip``` and hit enter
6. Afterwards, type ```pros c apply dlib``` and hit enter

Congrats! You have successfully installed dLib.
