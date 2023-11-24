// Create the charts when the web page loads
window.addEventListener('load', onload);

function onload(event){
  chartT = createTemperatureChart();
  chartH = createHumidityChart();
  chartP = createPressureChart();
}

// Create Temperature Chart
function createTemperatureChart() {
  var chart = new Highcharts.Chart({
    chart:{ 
      renderTo:'chart-temperature',
      type: 'spline' 
    },
    series: [
      {
        name: 'MQ135 Gas Sensor'
      }
    ],
    title: { 
      text: undefined
    },
    plotOptions: {
      line: { 
        animation: false,
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
        text: 'Particles Per Meter' 
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
      name: 'Noise Sensor'
    }],
    title: { 
      text: undefined
    },    
    plotOptions: {
      line: { 
        animation: false,
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
        text: 'Noise (dB)' 
      }
    },
    credits: { 
      enabled: false 
    }
  });
  return chart;
}