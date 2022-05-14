# CDH Firmware Update

## Update Manager States:

![CDH Software Update States](/docs/resources/CDH_FW_CommandFlow.png "The CDH Software Update States")

### IDLE: 
In this state nothing happens... During most operations the update manager can be brought back to this state.
	
### RX_FW(Receive Firmware):
In this state the update manager will accept firmware updates. The manager will allocate a file and write incoming firmware data to this file. We will remain in this state until the firmware file is fully received, and basic verification using crc32 for the full file is complete. If there is problems, the manager can accept new data for the file until it is complete.

### PRE-VERIFY:
In this state the update manager will verify the golden and update image on the data flash, as well as the program flash. It will also sync the program and data flash.

### ARMED:
From this state a firmware update can be executed. The main purpose is to require a 2nd command before initiating the update, to make sure it is intentional. Note that we cannot get to this state without first verifying the firmware.

### UPDATE:
In this state the update is executed. The CDH will reboot automatically.

### POST-VERIFY:
In this state we check that the currently running firmware matches the update firmware. We should automatically enter this state when rebooting after an upgrade, but this can be done manually. This can indicate whether the update was successful or if we reverted to the golden image.

## Instructions
 1. Follow the instructions in the CDH README to prepare an update file.
 2. In the Iris Terminal(the terminal) run the cdhUploadFw command, giving the path to the firmware as well as 0 or 1 to indicate whether this is to replace the "golden" or "update" firmware image.
 3.  If the upload works without errors, then run the cdhArmFw command to verify the firmware and arm the update system.
 4. Then run the cdhUpgradeFw command, passing 1 as the argument. This will start an upgrade to the "upgrade" firmware. 
 5. Then run  cdhUpgradeFwConfirm command  to confirm and execute the upgrade. CDH will then restart and should automatically verify the update operation. Run the cdhFwPostVerify and cdhCurrentFw to confirm if needed.