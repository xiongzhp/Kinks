angular.module('KinksApp')
.directive('captchaCheck', function(captchaService) {  
  return {
    require: 'ngModel',
    link: function(scope, elem, attrs, ctrl) {
      scope.busy = false;
      scope.$watch(attrs.ngModel, function(value) {
        
        // hide old error messages
        ctrl.$setValidity('notMatch', true);
        
        if (!value || value.length!=4) {
          // incorrect length
          return;
        }
        
        scope.busy = true;
        captchaService.check_code(scope['captcha_id'], value)
          .success(function() {
            scope.busy = false;
          })
          .error(function(res) {
            if (res.notMatch){
              ctrl.$setValidity('notMatch', false);
            }
            scope.busy = false;
          });
      })
    }
  }
});