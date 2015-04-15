
#include "GateRTPhantom.hh"
#include "GateRTPhantomMessenger.hh"
#include "GateObjectStore.hh"
#include "GateSourceMgr.hh"
#include "GateVoxelBoxParameterized.hh"
#include "GateCompressedVoxelParameterized.hh"
#include "GateSourceVoxellized.hh"
#include "GateVSourceVoxelReader.hh"
#include "GateRegularParameterized.hh"
#include "GateVSource.hh"

GateRTPhantom::GateRTPhantom( G4String name)
{ isFirst = true;
  XDIM = 0;
  YDIM = 0;
  ZDIM = 0;
  ZDIM_OUTPUT = 0;
  pixel_width = G4ThreeVector(0.,0.,0.);
   m_VerboseLevel = 5;
    IsVoxellized = 0;
    IsEnabled    = 0;
    IsInitialized = 0;
    m_messenger = 0;
    m_name = name;
   m_inserter = 0;
   itsGReader = 0;
   itsSReader = 0;
   p_cK = 1;

  m_messenger = new GateRTPhantomMessenger(this);
}

GateRTPhantom::~GateRTPhantom()
{
delete m_messenger;
}



void GateRTPhantom::Compute(G4double){;}

G4int GateRTPhantom::IsAttachedTo(GateVVolume* aOI)
{G4int res = 0;
 if (m_inserter == aOI) res = 1;
 return res;
}

void GateRTPhantom::AttachToSource(G4String aname)
{
if ( IsEnabled == 0 )
{
G4cout << " The Object RTPhantom " << m_name << " is NOT enabled. You cannot attach it to a Source. IGNORED" << Gateendl;
return;
}
GateVSource *Source = GateSourceMgr::GetInstance()->GetSourceByName(aname);

if ( Source !=0 && IsVoxellized == 1 )
   { GateSourceVoxellized * SourceVoxl = (GateSourceVoxellized *) Source;

     if ( SourceVoxl  != 0 ) 
     {itsSReader = SourceVoxl->GetReader();}  
     else 
           {G4cout << " GateRTPhantom::AttachToSource WARNING : The Source " << Source->GetName() << " is NOT Voxellized. IGNORED."<<Gateendl;}
     
   }
else
{  G4cout <<   " GateRTPhantom::AttachToSource WARNING : The Source " << Source->GetName() << " does NOT exist. IGNORED." << Gateendl;

if ( IsVoxellized == 0 ) {
G4cout <<"GateRTPhantom::AttachToSource WARNING : The RTPhantom Object " << m_name << " is not of Voxellized Type. You cannot attach it to a Voxellized Source" << Gateendl;}
}

G4cout <<"GateRTPhantom::AttachToSource : The RTPhantom Object " << m_name << " has a Source Reader " << itsSReader << " from source " << Source->GetName()<<Gateendl;

}

void GateRTPhantom::Disable()
{IsEnabled = 0;}

void GateRTPhantom::AttachToGeometry(G4String aname)
{

if ( m_inserter !=0 ){G4cout << " GateRTPhantom::AttachToGeometry : WARNING : The RTPhantom "<<m_name<<" is already attached to Inserter Object "<<m_inserter->GetObjectName()<<Gateendl;}


if ( IsEnabled == 0 )
{
G4cout << " The Object RTPhantom " << m_name << " is NOT enabled. You cannot attach it to a Geometric Object. IGNORED" << Gateendl;
return;
} else { G4cout << " The Object RTPhantom " << m_name << " is enabled."<< Gateendl;}

  if (m_VerboseLevel > 2)
    G4cout << "GateRTPhantom::AttachToGeometry" << Gateendl;

//Describe();

GateVVolume * aInserter = GateObjectStore::GetInstance()->FindCreator(aname);

if ( aInserter != 0 )
   {
    m_inserter  =  aInserter;  
GateVoxelBoxParameterized *VBParamIns = dynamic_cast<GateVoxelBoxParameterized *> (aInserter);
GateCompressedVoxelParameterized* CVParamIns = dynamic_cast<GateCompressedVoxelParameterized*> (aInserter);
GateRegularParameterized* CRParamIns = dynamic_cast<GateRegularParameterized*> (aInserter);

GateVGeometryVoxelReader* GReader = 0;

if ( VBParamIns != 0 ) GReader = VBParamIns->GetReader();
if ( CVParamIns != 0 ) GReader = CVParamIns->GetReader();
if ( CRParamIns != 0 ) GReader = CRParamIns->GetReader();
itsGReader = GReader;
if ( GReader ==0 )G4cout << " GateRTPhantom::AttachTo ERROR : No Geometry Reader is associated to parameterized Object "<< aname<<Gateendl;

   }
    
else { G4cout << " GateRTPhantom::AttachToGeometry ERROR : Inserter Object "<< aname <<" does not exist ! IGNORED !!!!" << Gateendl;}

if( m_inserter != 0 )
{
G4cout << " GateRTPhantom::AttachToGeometry : The RTPhantom "<<m_name<<" is now ATTACHED to Inserter Object "<< m_inserter->GetObjectName() <<" : OK !!!" << Gateendl;
}


}

void GateRTPhantom::Describe()
{
G4cout << " GateRTPhantom Name is                            " << m_name << Gateendl;

if( itsSReader != 0 )
{G4cout << "             is attached to Source Reader      " << itsSReader->GetName()<<Gateendl;}

if( itsGReader != 0 )
{G4cout << "             is attached to Geometry Reader " << itsGReader->GetName()<<Gateendl;}
G4String answer = "No";
if (IsVoxellized == 1) answer= "Yes";
G4cout << "             is Voxellized                                 " << answer<<Gateendl;
answer = "No";
if (IsEnabled == 1) answer="Yes";
G4cout << "             is Enabled                                    " << answer<<Gateendl;
}


