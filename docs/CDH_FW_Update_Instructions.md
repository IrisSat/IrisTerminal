# CDH Firmware Update

## Update Manager States:

![CDH Software Update States](/docs/resources/CDH_FW_CommandFlow.png "The CDH Software Update States")

### IDLE: 
In this state nothing happens... During most operations the update manager can be brought back to this state.
	
### RX_FW(Receive Firmware):
In this state the update manager will accept firmware updates. The manager will allocate a file and write incoming firmware data to this file. We will remain in this state until the firmware file is fully received, and basic verification using crc32 for the full file is complete. If there is problems, the manager can accept new data for the file until it is complete.

### PRE-VERIFY:
In this state the update manager will verify the golden and update image on the data flash, as well as the program flash. It will also sync the program and data flash.
Ideally this is run periodically, to maintain the integrity of the firmware before it needs a new upload.

### ARMED:
From this state a firmware update can be executed. The main purpose is to require a 2nd command before initiating the update, to make sure it is intentional. Note that we cannot get to this state without first verifying the firmware. There is a timeout in this state, if left too long in the arm state, the memory could be corrupted after we have already verified it. Therefore after the timeout period the state is set back to idle and the firmware must be verified again.

### UPDATE:
In this state the update is executed. The CDH will reboot automatically. Once in this state, there is a final execute confirm command that must be sent to actually do the upgrade, as extra protection.

### POST-VERIFY:
In this state we check that the currently running firmware matches the update firmware. We should automatically enter this state when rebooting after an upgrade, but this can be done manually. This can indicate whether the update was successful or if we reverted to the golden image. It seems this only checks the fpga design version, so when uploading new firmware the software version should be updated as an aditional check, to tell if the update worked.

## Instructions
 1. Follow the instructions in the CDH README to prepare an update file.
 2. In the Iris Terminal(the terminal) run the "cdhUploadFw" command, giving the path to the firmware as well as 0 or 1 to indicate whether this is to replace the "golden" or "update" firmware image.
 3.  If the upload works without errors, then run the "cdhArmFw" command to verify the firmware and arm the update system. This can take several minutes to verify and 
 4. Then run the "cdhUpgradeFw" command, passing 1 as the argument. This will start an upgrade to the "upgrade" firmware. 
 5. Then run  "cdhConfirmFw" command  to confirm and execute the upgrade. CDH will then restart and should automatically verify the update operation. Run the cdhFwPostVerify and cdhCurrentFw to confirm if needed.

 **NOTE: Make sure the upgrade is done with well tested firmware. The programming recovery does not protect against bad firmware! It only provides recovery in the case of powerloss during the upgrade process.**

- Note: The system will not do an update unless there is a good update and golden image uploaded on the sattelite. So the golden image can/should be uploaded on the ground before, hopefully to avaoid having to upload it.

 ## Troubleshooting and Extra Info

 - You can use the "checksum" command to compute the checksum of an upgrade file on you PC. Then use the "cdhChecksumFile" and "cdhChecksumFlash" to confirm the files on the data or program flash.
 - You can use the cdhGetFwState command to see the current state of the system. This can be used to check the completion of the upload, pre and post verify steps, since the system should transit to anthother state once done.
