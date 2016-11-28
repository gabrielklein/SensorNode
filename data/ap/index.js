/**
 * We have clicked on a tab.
 */
function clickedTab(t1, t2) {

    for (var i = 1; i <= 4; i++) {
        $("#tab" + i).removeClass('tabsel');
        $("#tab" + i + "a").removeClass('tabsel');
    }

    $(t1).addClass('tabsel');
    $(t2).addClass('tabsel');
}

function showHideLayer() {

    if (!$("#ip_dhcp").is(':checked')) {
        // Unckecked
        $("#ipdhcpspan").show();
    } else {
        // Checked
        $("#ipdhcpspan").hide();
    }

    if (!$("#remote_use").is(':checked')) {
        // Unckecked
        $("#remotespan").hide();
    } else {
        // Checked
        $("#remotespan").show();
    }

}


/**
 * Functions when we are ready.
 */
$(document).ready(function() {

    $.get({
            url: "config.do",
            dataType: "text"
        })
        .done(function(data) {
            var d = ("" + data).split("\n");
            for (var i = 0; i < d.length; i++) {
                var e = d[i].indexOf("=");
                if (e > 1) {
                    var key = d[i].substring(0, e);
                    var v = d[i].substring(e + 1);
                    $("#" + key).val(v);
                    try {
                        if (v == "1") {
                            $("#" + key).prop('checked', true);
                        }
                        if (v == "0") {
                            $("#" + key).prop('checked', false);
                        }
                    } catch (e) {

                    }
                }
            }
            setTimeout("showHideLayer();", 100);
            //$("#savewireless").attr('disabled', null);
        }).fail(function(xhr, status, error) {
            alert("Cannot get configuration.\n" + error);
            //$("#savewireless").attr('disabled', null);
        });

    // -------------------
    // Manage tabs -------
    // -------------------
    // Activate first tab
    clickedTab("#tab1", "#tab1a");

    $("#tab1a").click(function() {
        clickedTab("#tab1", "#tab1a");
        return false;
    });

    $("#tab2a").click(function() {
        clickedTab("#tab2", "#tab2a");
        return false;
    });

    $("#tab3a").click(function() {
        clickedTab("#tab3", "#tab3a");
        return false;
    });

    $("#tab4a").click(function() {
        clickedTab("#tab4", "#tab4a");
        return false;
    });

    // -------------------
    // About panel -------
    // -------------------
    $("#reboot").click(function() {
        $.get("reboot.do")
            .done(function(data) {
                document.location.href = "https://github.com/gabrielklein/SensorNode";
            }).fail(function(xhr, status, error) {
                alert("Sorry cannot reboot.\n" + error);
                //$("#savewireless").attr('disabled', null);
            });
    });

    $("#reset").click(function() {
        $.get("reset.do")
            .done(function(data) {
                document.location.reload();
            }).fail(function(xhr, status, error) {
                alert("Sorry cannot reload page.\n" + error);
                //$("#savewireless").attr('disabled', null);
            });
    });

    // -------------------
    // Configure wireless -------
    // -------------------

    $("#wireless_save").click(function() {

        $.post("config.post", {
                wireless_password: $("#wireless_password").val(),
                wireless_ssid: $("#wireless_ssid").val()
            })
            .done(function(data) {
                alert("Data saved");
                //$("#savewireless").attr('disabled', null);
            }).fail(function(xhr, status, error) {
                alert("Sorry cannot save.\n" + error);
                //$("#savewireless").attr('disabled', null);
            });

        return false;
    });

    $("#wireless_scan").click(function() {
        $("#wireless_scan").attr('disabled', 'disabled');

        $('#scanresult').empty();
        $('#scanresult').append("Please wait...");

        $.get({
                url: "scan.do",
                dataType: "text"
            })
            .done(function(data) {

                $("#wireless_scan").attr('disabled', null);
                $('#scanresult').empty();

                var d = ("" + data).split("\n");
                for (var i = 0; i < d.length; i++) {
                    var e = d[i].split(",");
                    if (e.length > 3) {

                        var rssi = e[0];
                        var enc = e[1];
                        var channel = e[2];
                        // Potential bug if we have , in the ssid
                        var ssid = e[3];

                        var but = $('<button></button>').text(ssid);
                        (function(ssid) {
                            but.click(function() {
                                $('#wireless_ssid').val(ssid);
                            });
                        }(ssid));

                        $('#scanresult').append(but);

                        var enc = "?" + enc;
                        switch (enc) {
                            case 2:
                                enc = "TKIP (WPA)";
                                break
                            case 5:
                                enc = "WEP";
                                break
                            case 4:
                                enc = "CCMP (WPA)";
                                break
                            case 7:
                                enc = "NONE";
                                break
                            case 8:
                                enc = "AUTO";
                                break
                        }

                        var div = $('<span></span>').addClass('small').text(' (Channel: ' + channel + ", RSSI: " + rssi + ', Enc: ' + enc + ')');
                        $('#scanresult').append(div);
                        $('#scanresult').append("<br />");
                    }
                }

            })
            .fail(function(xhr, status, error) {
                alert("Cannot scan network.\n" + error);
                //$("#savewireless").attr('disabled', null);
            });


        return false;
    });

    // -------------------
    // Configure ip -------
    // -------------------

    $("#ip_save").click(function() {


        $.post("config.post", {
                ip_dhcp: $("#ip_dhcp").is(':checked') ? "1" : "0",
                ip_ip: $("#ip_ip").val(),
                ip_subnet: $("#ip_subnet").val(),
                ip_gateway: $("#ip_gateway").val(),
                ip_dns: $("#ip_dns").val()
            })
            .done(function(data) {
                alert("Data saved");
                //$("#savewireless").attr('disabled', null);
            }).fail(function(xhr, status, error) {
                alert("Sorry cannot save.\n" + error);
                //$("#savewireless").attr('disabled', null);
            });
        //      $.ajax({
        //  type: "POST",
        //  url: url,
        //  data: data,
        //  success: success,
        //  dataType: dataType
        //});
        return false;
    });

    $("#ip_dhcp").change(function() {
        setTimeout("showHideLayer();", 100);
        return true;
    });

    // -------------------
    // Configure remote -------
    // -------------------

    $("#remote_save").click(function() {


        $.post("config.post", {
                remote_use: $("#remote_use").is(':checked') ? "1" : "0",
                remote_url: $("#remote_url").val()
            })
            .done(function(data) {
                alert("Data saved");
                //$("#savewireless").attr('disabled', null);
            }).fail(function(xhr, status, error) {
                alert("Sorry cannot save.\n" + error);
                //$("#savewireless").attr('disabled', null);
            });
        //      $.ajax({
        //  type: "POST",
        //  url: url,
        //  data: data,
        //  success: success,
        //  dataType: dataType
        //});
        return false;
    });

    $("#remote_use").change(function() {
        setTimeout("showHideLayer();", 100);
        return true;
    });








});
