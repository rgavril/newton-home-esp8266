var app = new Vue({
    el: '#app',
    data: {
        activeTab: 1,
        settings: []
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