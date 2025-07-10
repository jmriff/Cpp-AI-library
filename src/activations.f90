
! TODO: Fix Fortran <-> C linkage with activations.f90 and activations.h

module activations
   use, intrinsic :: iso_c_binding

   implicit none

contains

! Activation functions

   pure function nn_tanh(x) bind(c, name='nn_tanh')
      implicit none
      real(kind=C_DOUBLE), intent(in) :: x
      real(kind=C_DOUBLE) :: nn_tanh

      nn_tanh = tanh(x)
   end function nn_tanh

   pure function nn_sigmoid(x) bind(c)
      implicit none
      real(kind=C_DOUBLE), INTENT(IN) :: x
      real(kind=C_DOUBLE) :: nn_sigmoid

      nn_sigmoid = 1.0/(1.0 + exp(-x))
   end function nn_sigmoid

   pure function nn_fsigmoid(x) bind(c)
      implicit none
      real(kind=C_DOUBLE), INTENT(IN) :: x
      real(kind=C_DOUBLE) :: nn_fsigmoid

      nn_fsigmoid = x/(1.0 + abs(x))
   end function nn_fsigmoid

   pure function nn_relu(x) bind(c)
      implicit none
      real(kind=C_DOUBLE), INTENT(IN) :: x
      real(kind=C_DOUBLE) :: nn_relu

      !  nn_relu = x*(x > 0)
      if (x > 0) then
         nn_relu = x
      else
         nn_relu = 0.0
      end if
   end function nn_relu

   ! Derivative of activation functions
   pure function nn_dtanh(x) bind(c)
      implicit none
      real(kind=C_DOUBLE), INTENT(IN) :: x
      real(kind=C_DOUBLE) :: nn_dtanh

      ! nn_dtanh = pow(1/cosh(x), 2)
      nn_dtanh = (1.0/COSH(X))**2
   end function nn_dtanh

   pure function nn_dsigmoid(x) bind(c)
      implicit none
      real(kind=C_DOUBLE), INTENT(IN) :: x
      real(kind=C_DOUBLE) :: nn_dsigmoid
      real(kind=C_DOUBLE) :: sig

      sig = nn_sigmoid(x)
      nn_dsigmoid = sig*(1.0 - sig)
   end function nn_dsigmoid

   pure function nn_dfsigmoid(x) bind(c)
      implicit none
      real(kind=C_DOUBLE), INTENT(IN) :: x
      real(kind=C_DOUBLE) :: nn_dfsigmoid

      nn_dfsigmoid = 1.0/((abs(x) + 1.0)**2)
   end function nn_dfsigmoid

   pure function nn_drelu(x) bind(c)
      implicit none
      real(kind=C_DOUBLE), INTENT(IN) :: x
      real(kind=C_DOUBLE) :: nn_drelu

      ! nn_drelu = (x > 0) ?1.0:0.0
      if (x > 0) then
         nn_drelu = 1.0
      else
         nn_drelu = 0.0
      end if
   end function nn_drelu
end module activations

