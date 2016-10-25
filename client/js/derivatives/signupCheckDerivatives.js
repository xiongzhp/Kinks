angular.module('KinksApp')
.directive('uniqueUsername', function(userService) {  
  return {
    require: 'ngModel',
    link: function(scope, elem, attrs, ctrl) {
      scope.busy = false;
      scope.$watch(attrs.ngModel, function(value) {
        
        // hide old error messages
        ctrl.$setValidity('isTaken', true);
        ctrl.$setValidity('invalidChars', true);
        
        if (!value) {
          // don't send undefined to the server during dirty check
          // empty username is caught by required directive
          return;
        }
        
        scope.busy = true;
        userService.check_username(value)
          .success(function(data) {
            scope.busy = false;
          })
          .error(function(data) {
            
            // display new error message
            if (data.isTaken) {
              ctrl.$setValidity('isTaken', false);
            } else if (data.invalidChars) {
              ctrl.$setValidity('invalidChars', false);
            }

            scope.busy = false;
          });
      })
    }
  }
})
.directive('match', function () {
  return {
    require: 'ngModel',
    link: function (scope, elem, attrs, ctrl) {
      
      scope.$watch('[' + attrs.ngModel + ', ' + attrs.match + ']', function(value){
        ctrl.$setValidity('match', value[0] === value[1] );
      }, true);

    }
  }
});
