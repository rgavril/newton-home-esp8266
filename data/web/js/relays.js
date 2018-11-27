var app = new Vue({
    el: '#app',
    data: {
      relays: []
    },
    
    methods: {
        getStates: function() {
            var vm = this;
            axios.get('/relays/get')
                .then(function (response) {
                    vm.relays = response.data;
                });
            setTimeout(function(){ 
                vm.getStates();
            }, 2000);
        }
    },

    mounted: function() {
        this.getStates();
    }
  })