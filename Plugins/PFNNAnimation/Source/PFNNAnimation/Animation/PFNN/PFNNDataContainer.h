// Copyright 2018 Sticks & Stones. All Rights Reserved.

#pragma once

#include "PhaseFunctionNeuralNetwork.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Async/AsyncWork.h"

#include "PFNNDataContainer.generated.h"

class UPhaseFunctionNeuralNetwork;

/**
 * 
 */
UCLASS()
class PFNNANIMATION_API UPFNNDataContainer : public UObject
{
	GENERATED_BODY()

public:

	UPFNNDataContainer(const FObjectInitializer& arg_ObjectInitializer);

	~UPFNNDataContainer();

	/*
	* @Description Load in the Phase Function Neural Network.
	*/
	void LoadNetworkData(EPFNNMode arg_Mode);
	/*
	* @Description Puts data into the passed PFNN object
	* @Param[out] arg_PFNN, The object that will recieve the data
	*/
	void GetNetworkData(UPhaseFunctionNeuralNetwork& arg_PFNN);
	bool IsDataLoaded() const;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "True"))
	bool bIsDataLoaded;

	/*
	* @Description Load weights for the Phase Function Neural Network
	* @Param[in] arg_A, Base of eigen array
	* @Param[in] arg_Rows, Number of rows in the matrix
	* @Param[in] arg_Cols, Number of colums in the matrix
	* @Param[in] arg_FileName, The file path where to find the Neural Network data
	*/
	void LoadWeights(Eigen::ArrayXXf& arg_A, const int arg_Rows, const int arg_Cols, const FString arg_FileName, ...);
	/*
	* @Description Load weights for the Phase Function Neural Network
	* @Param[in] arg_V, Base of eigen array
	* @Param[in] arg_Items, Items that need to be loaded in
	* @Param[in] arg_FileName, The file path where to find the Neural Network data
	*/
	void LoadWeights(Eigen::ArrayXf &arg_V, const int arg_Items, const FString arg_FileName, ...);

	//DO NOT CHANGE! These values determine the dimentions of the Neural Network. Changing them without knowing what you are doing will crash the engine.
	enum { XDIM = 342, YDIM = 311, HDIM = 512 };

	Eigen::ArrayXf Xmean, Xstd;
	Eigen::ArrayXf Ymean, Ystd;

	TArray<Eigen::ArrayXXf> W0, W1, W2;
	TArray<Eigen::ArrayXf>  b0, b1, b2;

	Eigen::ArrayXf  Xp, Yp;
	Eigen::ArrayXf  H0, H1;
	Eigen::ArrayXXf W0p, W1p, W2p;
	Eigen::ArrayXf  b0p, b1p, b2p;
	//Ending of things that you should not change.
};

class FPFNNDataLoader : public FNonAbandonableTask
{

public:

	FPFNNDataLoader(UPFNNDataContainer* arg_PFNNDataContainer);

	~FPFNNDataLoader();

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(PFNNDataLoader, STATGROUP_ThreadPoolAsyncTasks)
	}

	void DoWork();

private:

	UPROPERTY()
	UPFNNDataContainer* PFNNDataContainer;

};