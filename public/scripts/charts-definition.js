// Create the charts when the web page loads
window.addEventListener('load', onload);

function onload(event){
  chartT = createTemperatureChart();
  chartH = createHumidityChart();
}

Highcharts.setOptions({
  time: {
    timezoneOffset: -7 * 60
  }
});

// Create Temperature Chart
function createTemperatureChart() {
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-temperature',
      type: 'spline' 
    },
    series: [
      {
        name: 'Toilet Usage'
      }
    ],
    title: { 
      text: undefined
    },
    plotOptions: {
      line: { 
        animation: true,
        dataLabels: { 
          enabled: true 
        }
      },
      series: { 
        color: '#0f0f0f' 
      }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { 
        text: 'Toilet Usage' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}

// Create Humidity Chart
function createHumidityChart(){
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-humidity',
      type: 'spline'  
    },
    series: [{
      name: 'Flush Usage'
    }],
    title: { 
      text: undefined
    },
    plotOptions: {
      line: { 
        animation: true,
        dataLabels: { 
          enabled: true 
        }
      },
      series: { 
        color: '#0f0f0f' 
      }
    },
    xAxis: {
      type: 'datetime',
      dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
      title: { 
        text: 'Flush Usage' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}