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

}


/**
 * Functions when we are ready.
 */
$(document).ready(function() {

  $("#wireless_scan").attr('disabled', null);

  $.getJSON("ap/config")
    .done(function(data) {

      jQuery.each(data.config, function(key, v) {
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

      });

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


  // -------------------
  // About panel -------
  // -------------------
  $("#reboot").click(function() {
    $.getJSON("ap/reboot")
      .done(function(data) {
        $("#reboot").attr('disabled', 'disabled');
        setTimeout("document.location.reload();", 2000);
      }).fail(function(xhr, status, error) {
        alert("Sorry cannot reboot.\n" + error);
      });
  });

  $("#reset").click(function() {
    $.getJSON("ap/reset")
      .done(function(data) {
        $("#reset").attr('disabled', 'disabled');
        setTimeout("document.location.reload();", 2000);
      }).fail(function(xhr, status, error) {
        alert("Sorry cannot reload page.\n" + error);
        $("#savewireless").attr('disabled', null);
      });
  });

  // -------------------
  // Configure wireless -------
  // -------------------

  $("#wireless_save").click(function() {

    $.getJSON("ap/config", {
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

    $.getJSON("ap/scan")
      .done(function(data) {


        $("#wireless_scan").attr('disabled', null);
        $('#scanresult').empty();


        jQuery.each(data.ap, function(key, v) {

          var rssi = v.dbm;
          var bssid = v.bssid;
          var enc = parseInt(v.enc);
          var channel = v.channel;
          var ssid = v.ssid;

          var but = $('<button></button>').text(ssid);
          (function(ssid) {
            but.click(function() {
              $('#wireless_ssid').val(ssid);
            });
          }(ssid));

          $('#scanresult').append(but);

          var encs = "?" + enc;
          switch (enc) {
            case 2:
              encs = "TKIP (WPA)";
              break
            case 5:
              encs = "WEP";
              break
            case 4:
              encs = "CCMP (WPA)";
              break
            case 7:
              encs = "NONE";
              break
            case 8:
              encs = "AUTO";
              break
          }

          var div = $('<span></span>').addClass('small').text(' (Chan: ' + channel + ", " + rssi + ' db, ' + bssid + ', Enc: ' + encs + ')');
          $('#scanresult').append(div);
          $('#scanresult').append("<br />");
        });

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


    $.getJSON("ap/config", {
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


});
