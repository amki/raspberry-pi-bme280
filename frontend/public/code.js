
// All you need to do is pass your configuration as third parameter to the chart function
document.addEventListener('DOMContentLoaded',function(){
    var now = new Date().getTime()/1000;
    var from = now - 3600*24;
    $.getJSON( "/bmedata?from="+from+"&to="+now,function(data) {
        var temperature = [];
        var pressure = [];
        var humidity = [];
        var datetime = [];
        for(var i=0;i<data.rows.length;++i) {
            if(i % 3 == 0) {
                var row = data.rows[i];
                temperature.push(row["temperature"]);
                pressure.push(row["pressure"]);
                humidity.push(row["humidity"]);
                datetime.push(row["datetime"]);
            }
        }
        var tempCtx = $("#bme280");
        var myChart = new Chart(tempCtx, {
            type: 'line',
            data: {
                labels: datetime,
                datasets: [{
                    label: 'Temperature °C',
                    backgroundColor: "rgba(224,60,60,0.4)",
                    yAxisID: "y-axis-0",
                    data: temperature
                }, {
                    label: 'Humidity %',
                    backgroundColor: "rgba(66,134,244,0.4)",
                    yAxisID: "y-axis-1",
                    data: humidity
                }, {
                    label: 'Pressure hPa',
                    backgroundColor: "rgba(153,255,51,0.4)",
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
    });
});
