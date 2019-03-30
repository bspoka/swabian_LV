<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="15008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="lib" Type="Folder"/>
		<Item Name="swabian_vis" Type="Folder"/>
		<Item Name="channel_settings.ctl" Type="VI" URL="../channel_settings.ctl"/>
		<Item Name="load_channel_settings.vi" Type="VI" URL="../load_channel_settings.vi"/>
		<Item Name="make_stop_q.vi" Type="VI" URL="../make_stop_q.vi"/>
		<Item Name="register_swabian_events.vi" Type="VI" URL="../register_swabian_events.vi"/>
		<Item Name="save_channel_settings.vi" Type="VI" URL="../save_channel_settings.vi"/>
		<Item Name="stream_modes.ctl" Type="VI" URL="../stream_modes.ctl"/>
		<Item Name="swabian_actions.ctl" Type="VI" URL="../swabian_actions.ctl"/>
		<Item Name="swabian_channel_settings.ctl" Type="VI" URL="../swabian_channel_settings.ctl"/>
		<Item Name="swabian_disconnect.vi" Type="VI" URL="../swabian_disconnect.vi"/>
		<Item Name="swabian_event_refs.ctl" Type="VI" URL="../swabian_event_refs.ctl"/>
		<Item Name="swabian_events_tdef.ctl" Type="VI" URL="../swabian_events_tdef.ctl"/>
		<Item Name="swabian_initialize.vi" Type="VI" URL="../swabian_initialize.vi"/>
		<Item Name="swabian_local_data_tdef.ctl" Type="VI" URL="../swabian_local_data_tdef.ctl"/>
		<Item Name="swabian_q_tdef.ctl" Type="VI" URL="../swabian_q_tdef.ctl"/>
		<Item Name="Swabian_UI.vi" Type="VI" URL="../Swabian_UI.vi"/>
		<Item Name="unregister_swabian_events.vi" Type="VI" URL="../unregister_swabian_events.vi"/>
		<Item Name="update_chart_tdef.ctl" Type="VI" URL="../update_chart_tdef.ctl"/>
		<Item Name="update_status.vi" Type="VI" URL="../update_status.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Check if File or Folder Exists.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Check if File or Folder Exists.vi"/>
				<Item Name="compatCalcOffset.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatCalcOffset.vi"/>
				<Item Name="compatFileDialog.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatFileDialog.vi"/>
				<Item Name="compatOpenFileOperation.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatOpenFileOperation.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="FindCloseTagByName.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindCloseTagByName.vi"/>
				<Item Name="FindElement.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindElement.vi"/>
				<Item Name="FindElementStartByName.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindElementStartByName.vi"/>
				<Item Name="FindEmptyElement.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindEmptyElement.vi"/>
				<Item Name="FindFirstTag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindFirstTag.vi"/>
				<Item Name="FindMatchingCloseTag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/FindMatchingCloseTag.vi"/>
				<Item Name="NI_FileType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/lvfile.llb/NI_FileType.lvlib"/>
				<Item Name="NI_PackedLibraryUtility.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/LVLibp/NI_PackedLibraryUtility.lvlib"/>
				<Item Name="Open_Create_Replace File.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/Open_Create_Replace File.vi"/>
				<Item Name="ParseXMLFragments.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/ParseXMLFragments.vi"/>
				<Item Name="Read From XML File(array).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Read From XML File(array).vi"/>
				<Item Name="Read From XML File(string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Read From XML File(string).vi"/>
				<Item Name="Read From XML File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Read From XML File.vi"/>
				<Item Name="subTimeDelay.vi" Type="VI" URL="/&lt;vilib&gt;/express/express execution control/TimeDelayBlock.llb/subTimeDelay.vi"/>
				<Item Name="Write to XML File(array).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Write to XML File(array).vi"/>
				<Item Name="Write to XML File(string).vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Write to XML File(string).vi"/>
				<Item Name="Write to XML File.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/xml.llb/Write to XML File.vi"/>
			</Item>
			<Item Name="change_connect_button_state.vi" Type="VI" URL="../change_connect_button_state.vi"/>
			<Item Name="channel_data_tdef.ctl" Type="VI" URL="../channel_data_tdef.ctl"/>
			<Item Name="reference_checkbox_tdef.ctl" Type="VI" URL="../reference_checkbox_tdef.ctl"/>
			<Item Name="swabian_buffer_tdef.ctl" Type="VI" URL="../swabian_buffer_tdef.ctl"/>
			<Item Name="TTCSharp32.dll" Type="Document" URL="../../lib/TTCSharp32.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
