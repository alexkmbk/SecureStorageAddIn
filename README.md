# RDPClientInfo
1C:Enterprise addin to get a public IP address of RDP client

Example:   

   

	If AttachAddIn("CommonTemplate.RDPClientInfoAddin", "Component", AddInType.Native) Then
		RDPClientInfo = New("AddIn.Component.RDPClientInfo");			
		IP = RDPClientInfo.GetExternalIP();
	EndIf;	

   
