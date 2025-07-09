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

   double nn_fsigmoid(double x)
   {
   return x/(1 + fabs(x)); 
   }

   double nn_relu(double x)
   {
   return x*(x > 0); 
   }

   //Derivative of activation functions
   double nn_dtanh(double x)
   {
   return pow(1/cosh(x), 2); 
   }

   double nn_dsigmoid(double x)
   {
   double sig = nn_sigmoid(x); 
   return sig*(1 - sig); 
   }

   double nn_dfsigmoid(double x)
   {
   return 1/pow((fabs(x) + 1), 2); 
   }

   double nn_drelu(double x)
   {
   return(x > 0) ?1.0:0.0; 
   }

end module activations
