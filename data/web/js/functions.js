function getValues() {
	$.get('http://192.168.0.17/api/time/', function (data) {
		console.log(JSON.stringify(data));
		$('#selectTZ').val(data.TimeZone);
		$('#selectNTP').val(data.NTPServer);
	});
	$.get('http://192.168.0.17/api/wifi/', function (data) {
		console.log(JSON.stringify(data));
		$('#ssid').val(data.ssid);
		$('#pass').val(data.pass);
		$('#ip').val(data.ip);
		$('#mask').val(data.mask);
		$('#gateway').val(data.gw);
		$('#dnsname').val(data.dns);
	});
	$.get('http://192.168.0.17/api/other/', function (data) {
		console.log(JSON.stringify(data));
		isChecked = data.EnableDynamicBright == "1" ? true : false
		$('#enableDynamic').prop('checked', isChecked);
		handleCheckbox(isChecked);
		if (isChecked) {
			$('#maxBright').val(data.MaxBright);
		} else {
			$("#staticBright").val(data.StaticBrightness)
		}
		$('#defaultStart').val(data.StartMode)
	});
}

function handleCheckbox(check) {
	if (check) {
		$("#maxBright").prop("disabled", false);
		$("#staticBright").prop("disabled", true);
	} else {
		$("#maxBright").prop("disabled", true);
		$("#staticBright").prop("disabled", false);
	}

}