## Repository containing the developments for the ESA - Summer Of Code In Space 2017

### STAGE 1:

Stage 1 - Create a simple co-simulation model in INTO-CPS, consisting of TWO VDM applications talking together. One playing the role of the controller, the other playing the role of the plant. 
Like from SOCIS 2016, the focus is to make these models as simple as possible, however using at least one of each basic FMI data types in both directions of the interface. Trivial manipulations of data.  

#### Requirements:
The TASTE VM has to download and install/update the following packages:

- Modelio 3.6.1
- INTO-CPS 3.1.3
- Overture 2.4.8

Add the required components to Modelio and Overture:

Modelio  
-------
In the system requirements, Modelio suggests 3 GB of memory. TASTE VM starts with 1 GB of memory. Increase the memory size from the VirtualBox menu.  

- Download Modelio at the following [link](https://www.modelio.org/download/send/16-modelio-3-4-1/29-modelio-3-4-1-linux-32-bit.html)
- `wget http://forge.modelio.org/attachments/download/16192/SysMLArchitect_3.4.04.jmdac`
- `wget http://forge.modelio.org/attachments/download/19524/INTOCPS_1.2.06.jmdac`

Overture 
--------


### STAGE 2:

