//--Author      DI Glazier 30/06/2014
//--Rev
//--Update
//--Description
//HASPECT Event Reconstruction
//HSControl
//Configure the executable
//Compile and link required classes
//Set PROOF parameters
//Set entrylist if required

TProof* HSControl(Bool_t UsePROOF=kFALSE){
  //////////////Set path to THSParticle.h etc.
  TString HSANA=gSystem->Getenv("HSANA");
  gInterpreter->AddIncludePath(HSANA);
  gEnv->SetValue("Unix.*.Root.MacroPath:",TString(".:")+HSANA);
  //make uptodate version of the classes once, in proof just load them (+ compared to ++)
  gROOT->LoadMacro(HSANA+"/THSParticle.C++");
  gROOT->LoadMacro(HSANA+"/THSHisto.C++");
  gROOT->LoadMacro(HSANA+"/THSOutput.C++");
  gROOT->LoadMacro(HSANA+"/THSEventWeighter.C++");
   ////////////////////////////////////////////////
  
  TProof *plite=0;
   if(UsePROOF) {
     plite= TProof::Open("lite://");
     tree->SetProof();
     //  plite->SetParameter("PROOF_UseTreeCache", 0); //turn off caching =0
     // plite->SetParameter("PROOF_CacheSize", 1000M);
     plite->SetParameter("PROOF_UseMergers", 0);//seems to be a bit faster initialising, for when you have lots of histograms
     plite->SetParallel(2); //restrict to 2 workers
     plite->Load(HSANA+"/THSParticle.C+");
     plite->Load(HSANA+"/THSHisto.C+");
     plite->Load(HSANA+"/THSOutput.C+");
     plite->Load(HSANA+"/THSEventWeighter.C+");
   }
   return plite;
}
void HSEntryList(TString filename,TString listname="HSelist"){
  TDirectory* savedir=gDirectory;
  //use a previously created TEntryList
   TFile* elfile=new TFile(filename);
   TEntryList *el=(TEntryList*)elfile->Get(listname)->Clone();
   tree->SetEntryList(el);
   savedir->cd();
}
