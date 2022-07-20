<div style="text-align:center"><h1>
    Release Notes for SE_STSAFEA_API
    </h1></div>

<div style="text-align:center">Copyright &copy; 2021 STMicroelectronics</div>

![](_htmresc\st_logo.png)

## License

This software is licensed by ST under SOFTWARE LICENSE AGREEMENT (SLA0088), the "License"; You may not use this component except in compliance with the license. You may obtain a copy of the License at:

[SLA0088 License @ www.st.com](LICENCE.txt)

## Purpose

The SE_STSAFEA_API is a software component providing a set of high-level APIs to access STSAFE-A110 device features from a host controller, using STSAFE-A1xx Middleware.

![](_htmresc\architecture_2.png)

The STSAFE-A110 is a highly secure solution that acts as a secure element providing authentication and data management services to a local or remote host. 
It consists of a full turnkey solution with a secure operating system running on the latest generation of secure microcontrollers. 
The STSAFE-A110 can be integrated in IoT (Internet of things) devices, smart-home, smart-city and industrial applications, consumer electronics devices, consumables and accessories.

**STSAFE-A110 Key Features**:

- Authentication (of peripherals, IoT and USB Type-C devices)

- Secure channel establishment with remote host including transport layer security (TLS) handshake 

- Signature verification service (secure boot and firmware upgrade) 

- Usage monitoring with secure counters

- Pairing and secure channel with host application processor 

- Wrapping and unwrapping of local or remote host envelopes

- On-chip key pair generation 




Here is the list of references to user documents:

- [ STSAFE-A110 Datasheet](https://www.st.com/resource/en/datasheet/stsafe-a110.pdf) : Authentication state-of-the-art security for peripherals and IoT devices

  

![](_htmresc\STSAFE_A_logo.png)



<div style="text-align:center"><h1>
    Update History </h1></div>




## V1.0.0

### Main Changes:

##### First Release

*Additional Features:*

First official version for STSAFE-A110 devices. It includes the following APIs and examples:

| Init and status commands:                             |
| ----------------------------------------------------- |
| SE_Init                                               |
| SE_GetSecurity                                        |
| SE_GetSerial                                          |
| SE_GetKeySize                                         |
| SE_GetCommandStatus                                   |
| SE_GetMapping                                         |
| SE_GetRandom                                          |
| SE_GetInfo                                            |
| **Management Commands**:                              |
| SE_Reset                                              |
| SE_Hibernate                                          |
| SE_Echo                                               |
| SE_Refresh                                            |
| SE_Raw                                                |
| **Enablement commands:**                              |
| SE_GetState                                           |
| SE_Unlock                                             |
| SE_Pairing                                            |
| SE_GetPairingStatus                                   |
| SE_SetPairingKeys                                     |
| **Memory partition access:**                          |
| SE_Read                                               |
| SE_ReadRight                                          |
| SE_Update                                             |
| SE_UpdateRight                                        |
| **Local Secure Storage commands:**                    |
| SE_GenerateWrapKey                                    |
| SE_Wrap                                               |
| SE_Unwrap                                             |
| **Asymmetric primitive:**                             |
| SE_VerifySignature                                    |
| SE_GenerateSignature                                  |
| SE_GenerateKeyPair                                    |
| SE_DeriveSharedSecret                                 |
| **Application level key and certificate management:** |
| SE_StoreCertificate                                   |
| SE_GetCertificate                                     |
| SE_SetActiveKey                                       |
| SE_GetActiveKey                                       |
| SE_GetActiveCertificate                               |
| SE_SetActiveCertificate                               |
| **Examples:**                                         |
| stsafea_describe                                      |

  



------

For complete documentation on **STSAFE-A110** , visit: [[STSAFE-A110 @ www.st.com](https://www.st.com/en/secure-mcus/stsafe-a110.html)]

This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.



