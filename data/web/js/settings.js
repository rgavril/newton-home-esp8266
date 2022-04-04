var app = new Vue({
    el: '#app',
    data: {
        activeTab: 1,
        settings: {
            "network": {
                "ssid"    : "Razvan's Wi-Fi Network",
                "password": "test data",
                "hostname": "arduino-000471FE"
            },
            "mqtt": {
                "broker": "10.0.1.7",
                "port": "1883",
                "client_id": "arduino-000471FE",
                "topic": "arduino-000471FE",
                "user": "",
                "password": ""
            }
        }
    },
    
    methods: {
        loadSettings: function() {
            var vm = this;
            axios.get('/settings/get')
                .then(function (response) {
                    vm.settings = response.data;
                });
        },
        saveSettings: function() {
            var vm = this;
            axios.post("/settings/set", {
                settings: vm.settings
            });
        }
    },

    mounted: function() {
        this.loadSettings();
    }
  })