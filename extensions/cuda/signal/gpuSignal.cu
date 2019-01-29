// -*- C++ -*-
// -*- coding: utf-8 -*-
//
// Source Author: Liang Yu
// Copyright 2019

#include "gpuSignal.h"

#include <cuda_runtime.h>
#include <cufftXt.h>
#include "isce/cuda/helper_cuda.h"
#include "isce/cuda/helper_functions.h"

using isce::cuda::signal::gpuSignal;


/** Destructor **/
template<class T>
gpuSignal<T>::
~gpuSignal() {
    cufftDestroy(_plan);
}

/**
*  @param[in] signal input block of data
*  @param[out] spectrum output block of spectrum
*  @param[in] ncolumns number of columns of the block of data
*  @param[in] nrows number of rows of the block of data
 */
template<class T>
void gpuSignal<T>::
forwardRangeFFT(int ncolumns, int nrows)
{

    _configureRangeFFT(ncolumns, nrows);
    
    fftPlanForward(_rank, _n, _howmany,
                    _inembed, _istride, _idist,
                    _onembed, _ostride, _odist);

}

/**
*  @param[in] signal input block of data
*  @param[out] spectrum output block of spectrum
*  @param[in] ncolumns number of columns of the block of data
*  @param[in] nrows number of rows of the block of data
 */
template<class T>
void gpuSignal<T>::
forwardAzimuthFFT(int ncolumns, int nrows)
{

    _configureAzimuthFFT(ncolumns, nrows);
    
    fftPlanForward(_rank, _n, _howmany,
                    _inembed, _istride, _idist,
                    _onembed, _ostride, _odist);

}

/**
*  @param[in] rank rank of the transform (1: for one dimensional and 2: for two dimensional transform)
*  @param[in] size size of each transform (ncols: for range FFT, nrows: for azimuth FFT)
*  @param[in] howmany number of FFT transforms for a block of data (nrows: for range FFT, ncols: for azimuth FFT)
*  @param[in] inembed
*  @param[in] istride
*  @param[in] idist
*  @param[in] onembed
*  @param[in] ostride
*  @param[in] odist
*/
template <class T>
void gpuSignal<T>::
fftPlanForward(int rank, int *n, int howmany,
                int *inembed, int istride, int idist,
                int *onembed, int ostride, int odist)
{
    _cufft_type = CUFFT_C2C;
    checkCudaErrors(cufftCreate(&_plan));
    size_t worksize;
    checkCudaErrors(cufftMakePlanMany(_plan, rank, n, inembed,
                                      istride, idist, onembed, ostride, 
                                      odist, _cufft_type, _howmany, &worksize));
}

/** @param[in] ncolumns number of columns
*   @param[in] nrows number of rows
*/
template <class T>
void gpuSignal<T>::
_configureRangeFFT(int ncolumns, int nrows)
{
    _rank = 1;                  // dimensionality of transform
    _n = new int[1];
    _n[0] = ncolumns;

    _howmany = nrows;
    
    _inembed = new int[1];
    _inembed[0] = ncolumns;

    _istride = 1;
    _idist = ncolumns;
    
    _onembed = new int[1];
    _onembed[0] = ncolumns;

    _ostride = 1;
    _odist = ncolumns;
}

/** @param[in] ncolumns number of columns
*   @param[in] nrows number of rows
*/
template <class T>
void
gpuSignal<T>::
_configureAzimuthFFT(int ncolumns, int nrows)
{
    _rank = 1;
    _n = new int[1];
    _n[0] = nrows;

    _howmany = ncolumns;

    _inembed = new int[1];
    _inembed[0] = nrows;

    _istride = ncolumns;
    _idist = 1;

    _onembed = new int[1];
    _onembed[0] = nrows;

    _ostride = ncolumns;
    _odist = 1;
}

/** unnormalized forward transform
*  @param[in] input block of data
*  @param[out] output block of spectrum
*/
template<class T>
void isce::cuda::signal::gpuSignal<T>::
forward(std::valarray<std::complex<T>> &input, std::valarray<std::complex<T>> &output)
{
    size_t input_size = input.size()*sizeof(T)*2;
    size_t output_size = output.size()*sizeof(T)*2;

    // allocate device memory 
    T *d_input;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_input), input_size));
    T *d_output;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_output), output_size));

    // copy input
    checkCudaErrors(cudaMemcpy(d_input, &input[0], input_size, cudaMemcpyHostToDevice));

    // transform
    checkCudaErrors(cufftExecC2C(_plan, reinterpret_cast<cufftComplex *>(d_input),
                                reinterpret_cast<cufftComplex *>(d_input),
                                CUFFT_FORWARD));

    // copy output
    checkCudaErrors(cudaMemcpy(&output[0], d_input, input_size, cudaMemcpyDeviceToHost));
    
    cudaFree(d_input);
    cudaFree(d_output);
}

/** unnormalized forward transform
*  @param[in] input block of data
*  @param[out] output block of spectrum
*/
template<class T>
void isce::cuda::signal::gpuSignal<T>::
inverse(std::valarray<std::complex<T>> &input, std::valarray<std::complex<T>> &output)
{
    size_t input_size = input.size()*sizeof(T)*2;
    size_t output_size = output.size()*sizeof(T)*2;

    // allocate device memory 
    T *d_input;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_input), input_size));
    T *d_output;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_output), output_size));

    // copy input
    checkCudaErrors(cudaMemcpy(d_input, &input[0], input_size, cudaMemcpyHostToDevice));

    // transform
    checkCudaErrors(cufftExecC2C(_plan, reinterpret_cast<cufftComplex *>(d_input),
                                reinterpret_cast<cufftComplex *>(d_input),
                                CUFFT_INVERSE));

    // copy output
    checkCudaErrors(cudaMemcpy(&output[0], d_input, input_size, cudaMemcpyDeviceToHost));
    
    cudaFree(d_input);
    cudaFree(d_output);
}

/*
 each template parameter needs it's own declaration here
 */
template class gpuSignal<float>;
