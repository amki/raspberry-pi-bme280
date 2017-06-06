
var chart = null;
var updateInterval = null;
var lastUpdate = new Date().getTime()/1000

function graphHourly() {
    fetchAndGraph(1,1);
}

function graphDynamic() {
    var hours = parseFloat($("#hours").val());
    var skip = parseFloat($("#skip").val());
    fetchAndGraph(hours,skip);
}

function graphDaily() {
    fetchAndGraph(24,3);
}

function fetchAndGraph(hours, skip) {
    var now = new Date().getTime()/1000;
    var from = now - (3600 * hours);
    $.getJSON( "/bmedata?from="+from+"&to="+now,function(data) {
        drawChart(data.rows, skip);
    });
    if(updateInterval != null)
        clearInterval(updateInterval);
    updateInterval = setInterval(function(){
        console.log("Last update was "+lastUpdate);
        var now = new Date().getTime()/1000;
        $.getJSON( "/bmedata?from="+lastUpdate+"&to="+now,function(data) {
            updateChart(data.rows);
            lastUpdate = now;
        });
    }, 10000);
}

function updateChart(values) {
    for(var i=0;i<values.length;++i) {
        var val = values[i];
        chart.data.labels.push(val["datetime"]);
        chart.data.datasets[0].data.push(val["temperature"]);
        chart.data.datasets[1].data.push(val["humidity"]);
        chart.data.datasets[2].data.push(val["pressure"]);
    }
    chart.update();
}


function drawChart(values, skip) {
    if(chart != null) {
        chart.destroy();
    }
    var temperature = [];
    var pressure = [];
    var humidity = [];
    var datetime = [];
    for(var i=0;i<values.length;++i) {
        if(i % skip == 0) {
            var val = values[i];
            temperature.push(val["temperature"]);
            pressure.push(val["pressure"]);
            humidity.push(val["humidity"]);
            datetime.push(val["datetime"]);
        }
    }
    var tempCtx = $("#bme280");
    chart = new Chart(tempCtx, {
        type: 'line',
        data: {
            labels: datetime,
            datasets: [{
                label: 'Temperature °C',
                backgroundColor: "rgba(224,60,60,0.3)",
                yAxisID: "y-axis-0",
                data: temperature
            }, {
                label: 'Humidity %',
                backgroundColor: "rgba(66,134,244,0.3)",
                yAxisID: "y-axis-1",
                data: humidity
            }, {
                label: 'Pressure hPa',
                backgroundColor: "rgba(153,255,51,0.3)",
                yAxisID: "y-axis-2",
                data: pressure
            }
            ]
        },
        options: {
            scales: {
                yAxes: [{
                    "id": "y-axis-0",
                    position: 'left',
                    ticks: {
                        suggestedMin: 10,
                        suggestedMax: 30
                    }
                }, {
                    "id": "y-axis-1",
                    position: 'right',
                    ticks: {
                        suggestedMin: 0,
                        suggestedMax: 100
                    }
                }, {
                    "id": "y-axis-2",
                    position: 'left',
                    ticks: {
                        suggestedMin: 950,
                        suggestedMax: 1050
                    }
                }]
            },
            maintainAspectRation: false,
            elements: {
                line: {
                    tension: 0
                }
            },
            animation: {
                duration: 0
            },
        }
    });
}

// All you need to do is pass your configuration as third parameter to the chart function
document.addEventListener('DOMContentLoaded',function(){
    graphHourly();
});
