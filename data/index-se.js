/**
 * Variables
 */
var ledColorsArray = null;

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

/**
 * Refresh current time div
 */
function refreshCurrentTime() {
  $.getJSON("time/get")
    .done(function(data) {
      $("#time_result").empty();
      var div = $('<span></span>').text("UTC time: " + data.utc.hms + " ");
      $('#time_result').append(div);
      var br = $('<br />');
      $('#time_result').append(br);
      var div = $('<span></span>').text(data.local.timezone + " time: " + data.local.hms + " ");
      $('#time_result').append(div);

      $("#time_select").val(data.local.timezone);

    }).fail(function(xhr, status, error) {
      alert("Sorry cannot get current time.");
      //$("#savewireless").attr('disabled', null);
    });
}

/**
 * Refresh the timezone options
 */
function refreshTimezone() {
  $.getJSON({
      url: "time/timezone",
      dataType: "json"
    })
    .done(function(data) {

      var $el = $("#time_select");
      $el.empty();
      jQuery.each(data.tz, function(key, v) {
        $el.append($("<option></option>").attr("value", v.id).text("[" + v.hms + "] " + v.label));
      });

      refreshCurrentTime();

      //$("#savewireless").attr('disabled', null);
    }).fail(function(xhr, status, error) {
      alert("Cannot get timezone.\n" + error);
      //$("#savewireless").attr('disabled', null);
    });
}

/**
 * Refresh the timezone options
 */
function refreshLedConfig() {
  $.getJSON({
      url: "led/config",
      dataType: "json"
    })
    .done(function(data) {
      $("#leds_count").val(data.config.count);
    }).fail(function(xhr, status, error) {
      alert("Cannot get led configuration.\n" + error);
      //$("#savewireless").attr('disabled', null);
    });
}

/**
 * Refresh the timezone options
 */
function refreshLedColor2(min, max) {
  var c = 0;
  for (var i = min; i <= max; i++) {
    //<span id="led_span_0"><span id="led_label_0"></span>: <input class="jscolor" value="ffffff" id="led_in_0" /></span><br />
    var $els = $("#led_span_" + c);
    var $ell = $("#led_label_" + c);
    var $eli = $("#led_in_" + c);
    $ell.text(i + " ");
    $eli.val(ledColorsArray.led["l" + i].substr(1));
    $els.show();

    $els.unbind('change');
    $els.bind('change', {
        ledid: i,
        ledobj: $eli
      },
      function(event) {
        var ledId = event.data.ledid;
        var ledVal = event.data.ledobj.val();

        $.getJSON({
            url: "led/set?l=" + ledId + "&rgb=" + ledVal,
            dataType: "json"
          })
          .done(function(data) {
            //
          }).fail(function(xhr, status, error) {
            alert("Cannot change color.\n" + error);
          });

      }
    );

    $eli.css("background-color", ledColorsArray.led["l" + i]);
    //style="background-image: none; background-color: rgb(255, 255, 255); color: rgb(0, 0, 0);"

    c++;
  }
  for (var i = c; i < 10; i++) {
    var $els = $("#led_span_" + i);
    $els.hide();
  }

}

/**
 * Refresh the timezone options
 */
function refreshLedColor() {
  $.getJSON({
      url: "led/get",
      dataType: "json"
    })
    .done(function(data) {

      ledColorsArray = data;
      var $el = $("#led_range");
      $el.empty();
      for (var i = 0; i < data.count; i += 10) {
        var maxx = ((i + 10) >= data.count ? (data.count - 1) : (i + 9));
        $el.append($("<a></a>").text("[" + i + "-" + maxx + "]").bind('click', {
            min: i,
            max: maxx
          },
          function(event) {
            refreshLedColor2(event.data.min, event.data.max);
          }
        ));
        $el.append($("<i></i>").text(" "));
      }
      $el.append($("<a></a>").text("[Refresh]").bind('click', {},
        function(event) {
          refreshLedColor();
        }
      ));

      var maxx = (10 >= data.count ? (data.count - 1) : 9);
      refreshLedColor2(0, maxx);

    }).fail(function(xhr, status, error) {
      alert("Cannot get led colors.\n" + error);
      //$("#savewireless").attr('disabled', null);
    });
}


/**
 * Refresh temperature
 */
function refreshTemperature() {
  $.getJSON({
      url: "temperature/get",
      dataType: "json"
    })
    .done(function(data) {

      var $el = $("#temp_result");
      $el.empty();
      jQuery.each(data.temp, function(key, v) {
        $el.append($("<div></div>").text("[" + v.model + ":" + v.addr + "] " + (v.humidity == null ? "" : v.humidity + "% ") + " " + v.tempC + "°C " + v.tempF + "°F"));
      });

      //$("#savewireless").attr('disabled', null);
    }).fail(function(xhr, status, error) {
      alert("Cannot get timezone.\n" + error);
      //$("#savewireless").attr('disabled', null);
    });
}

/**
 * Refresh the timezone options
 */
function refreshMqttConfig() {
  $.getJSON({
      url: "mqtt/config",
      dataType: "json"
    })
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

      setTimeout("showHideLayerMqtt();", 100);

    }).fail(function(xhr, status, error) {
      alert("Cannot get led configuration.\n" + error);
      //$("#savewireless").attr('disabled', null);
    });
}


/**
 * Functions when we are ready.
 */
$(document).ready(function() {

  refreshTimezone();

  refreshTemperature();

  refreshLedConfig();

  refreshLedColor();

  refreshMqttConfig();

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
  // System panel -------
  // -------------------
  $("#time_get").click(function() {
    refreshTimezone();
  });

  // -------------------
  // System panel -------
  // -------------------
  $("#temp_get").click(function() {
    refreshTemperature();
  });


  $("#time_save").click(function() {

    var t = $("#time_select").val();

    $.getJSON("time/config", {
        timezone: t
      })
      .done(function(data) {
        refreshTimezone();
        alert("Timezone saved.");
      }).fail(function(xhr, status, error) {
        alert("Cannot save timezone!\n" + error);
      });

    var u = $("#leds_count").val();

    $.getJSON("led/config", {
        count: u
      })
      .done(function(data) {
        refreshLedColor();
        alert("Led saved.");
      }).fail(function(xhr, status, error) {
        alert("Cannot save leds count!\n" + error);
      });



  });

  // -------------------
  // MQTT panel -------
  // -------------------
  $("#mqtt_save").click(function() {

    $.getJSON("mqtt/config", {
        mqtt_enable: $("#mqtt_enable").is(':checked') ? "1" : "0",
        mqtt_server: $("#mqtt_server").val(),
        mqtt_port: $("#wireless_ssid").val(),
        mqtt_user: $("#mqtt_user").val(),
        mqtt_password: $("#mqtt_password").val(),
        mqtt_topic_register: $("#mqtt_topic_register").val(),
        mqtt_topic_message: $("#mqtt_topic_message").val()
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

  function showHideLayerMqtt() {

    if (!$("#mqtt_enable").is(':checked')) {
      // Unckecked
      $("#mqttspan").hide();
    } else {
      // Checked
      $("#mqttspan").show();
    }

  }

  $("#mqtt_enable").change(function() {
    setTimeout(new function(){showHideLayerMqtt();}, 100);
    return true;
  });



});
