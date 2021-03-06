#define ThreePiParent_cxx
// The class definition in ThreePiParent.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("ThreePiParent.C")
// Root > T->Process("ThreePiParent.C","some options")
// Root > T->Process("ThreePiParent.C+")
//

#include "ThreePiParent.h"
#include <TH2.h>
#include <TStyle.h>


void ThreePiParent::Begin(TTree * /*tree*/)
{
   THSOutput::HSBegin(fInput,fOutput);
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

void ThreePiParent::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   THSOutput::HSSlaveBegin(fInput,fOutput);
  //if you want kinematic bins you must define fHisbins here
   //fHisbins=new TH*()
   if(fHisbins) fOutput->Add(fHisbins);
   //fHisbins->SetXTitle();//give useful axis name
   //fHisbins->SetYTitle();//give useful axis name
   THSHisto::ChangeNames();
   THSHisto::LoadCut("NoCut");
   THSHisto::LoadCut("Eg_gt_3"); 
   THSHisto::LoadHistograms();
}

Bool_t ThreePiParent::Process(Long64_t entry)
{
   THSOutput::HSProcessStart(entry);
//this selector has been configured to use a parent selector
//get the branch with something like fParent.b_beam->GetEntry(entry)
//then use the object like fParent.beam->P4().E();
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either ThreePiParent::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.
   GetEntry(entry); //lazy and slow, you can speed the code up by getting the branches you need to use instead
   fParent.GetEntry(GetParentEntry(entry)); //lazy and slow, optimise your own analysis by only getting the branches you need
   //Ready to do some analysis here, before the Fill
   
   //Int_t kinBin=GetKinBin();//if fHisbins is defined need to give this meaningful arguments
   FillHistograms("NoCut",0);
   if(fParent.beam->P4().E()>3) FillHistograms("Eg_gt_3",0);   
   THSOutput::HSProcessFill(); 


   return kTRUE;
}

void ThreePiParent::SlaveTerminate()
{
   THSOutput::HSSlaveTerminate();
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ThreePiParent::Terminate()
{
   THSOutput::HSTerminate();
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
void ThreePiParent::HistogramList(TString sLabel){
  TDirectory::AddDirectory(kFALSE); //do not add to current directory
  //now define all histograms and add to Output
  //label includes kinematic bin and additional cut name
  // e.g fOutput->Add(MapHist(new TH1F("Mp1"+sLabel,"M_{p1}"+sLabel,100,0,2)));
  //end of histogram list
  fOutput->Add(MapHist(new TH2F("Theta2pi_V_t"+sLabel,"#theta_{2pi} v t"+sLabel,90,0,180,50,0,5)));
   fOutput->Add(MapHist(new TH2F("Theta2piCM_V_LAB"+sLabel,"#theta_{2pi} CM V LAB"+sLabel,90,0,180,90,0,180)));
   fOutput->Add(MapHist(new TH2F("TwoPiMCM_V_LAB"+sLabel,"TwoPiM CM V LAB"+sLabel,100,0.5,2,100,0.5,2)));  
   TDirectory::AddDirectory(kTRUE); //back to normal
}
void ThreePiParent::FillHistograms(TString sCut,Int_t bin){
  fCurrCut=sCut;
  fCurrBin=bin;
  //Get histogram from list
  //Fill histogram
  TString sLabel;
  sLabel=sCut+fVecBinNames[bin];
  FindHist("Theta2pi_V_t"+sLabel)->Fill(TwoPiCM->Theta()*TMath::RadToDeg(),fParent.t);
  FindHist("Theta2piCM_V_LAB"+sLabel)->Fill(TwoPiCM->Theta()*TMath::RadToDeg(),fParent.TwoPiF->Theta()*TMath::RadToDeg());
  FindHist("TwoPiMCM_V_LAB"+sLabel)->Fill(TwoPiCM->M(),fParent.TwoPiF->M());  // e.g. FindHist("Mp1"+sLabel)->Fill(fp1->M());
}
