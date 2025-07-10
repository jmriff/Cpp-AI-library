! TODO: Fix Fortran <-> C linkage with activations.f90 and activations.h

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

        nn_fsigmoid = x/(1 + abs(x))
    end function nn_fsigmoid

    pure function nn_relu(x)
        implicit none
        REAL, INTENT(IN) :: x
        REAL :: nn_relu

        ! f3fe-hash's temporary fix, optimize it possible
        if (x > 0) then
            nn_relu = x
        else
            nn_relu = 0
        end if
    end function nn_relu

   ! Derivative of activation functions
   !double nn_dtanh(double x)
   !{
   !return pow(1/cosh(x), 2); 
   !}

   !double nn_dsigmoid(double x)
   !{
   !double sig = nn_sigmoid(x); 
   !return sig*(1 - sig); 
   !}

   !double nn_dfsigmoid(double x)
   !{
   !return 1/pow((fabs(x) + 1), 2); 
   !}

   !double nn_drelu(double x)
   !{
   !return(x > 0) ?1.0:0.0; 
   !}

end module activations
