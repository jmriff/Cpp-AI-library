module activations

   implicit none

contains

! Activation functions

   pure function nn_tanh(x)
      implicit none
      real, intent(in) :: x
      real :: nn_tanh

      nn_tanh = tanh(x)
   end function nn_tanh

   pure function nn_sigmoid(x)
      implicit none
      real, INTENT(IN) :: x
      REAL :: nn_sigmoid

      nn_sigmoid = 1/(1 + exp(-x))
   end function nn_sigmoid

   pure function nn_fsigmoid(x)
      implicit none
      REAL, INTENT(IN) :: x
      REAL :: nn_fsigmoid

      nn_fsigmoid = x/(1 + fabs(x))
   end function nn_fsigmoid

   pure function nn_relu(x)
      implicit none
      REAL, INTENT(IN) :: x
      REAL :: nn_relu

      nn_relu = x*(x > 0)
   end function nn_relu

   ! Derivative of activation functions
   pure function nn_dtanh(x)
      implicit none
      REAL, INTENT(IN) :: x
      REAL :: nn_dtanh

      nn_dtanh = pow(1/cosh(x), 2)
   end function nn_dtanh

   pure function nn_dsigmoid(x)
      implicit none
      REAL, INTENT(IN) :: x
      REAL :: nn_dsigmoid
      REAL :: sig

      sig = nn_sigmoid(x); 
      nn_dsigmoid = sig*(1 - sig)
   end function nn_sigmoid

   double nn_dfsigmoid(double x)
   {
   return 1/pow((fabs(x) + 1), 2); 
   }

   double nn_drelu(double x)
   {
   return(x > 0) ?1.0:0.0; 
   }

end module activations
