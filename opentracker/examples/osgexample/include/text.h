// pilfered from testTextTXFParam in the source/text folder of the opensg installation

/** Little wrapper for holding text data. */
#include "text_dep.h"
class TextStuff
{
public:
   TextStuff()
      : mFace(NULL),
        mFamilyName("SANS"),
        mNextFamily(0),
        mStyle(OSG::TextFace::STYLE_PLAIN),
        mTextGap(1),
        mTextureWidth(1024),
        mFaceSize(28),
        mLineSpacing(1.0f),
        mMaxExtent(0.0f),
        mGeoScale(1.0f),
     mOffset(0.0, 0.0)

   {;}

   // Initialize the scene structures and get everything going
   void initialize()
   {
     mFamilies.push_back("SANS");
     OSG::TextFaceFactory::the().getFontFamilies(mFamilies);  // Get list of all families

      mRootNode = OSG::Node::create();
      mTextGeom = OSG::Geometry::create();
      mTextMat = OSG::ChunkMaterial::create();
      mTextureChunk = OSG::TextureChunk::create();
      mBlendChunk = OSG::BlendChunk::create();

      OSG::CPEditor rne(mRootNode);
      OSG::CPEditor tge(mTextGeom);
      OSG::CPEditor tme(mTextMat);
      OSG::CPEditor tce(mTextureChunk);
      OSG::CPEditor bce(mBlendChunk);

      // XXX: Setup a default face to use

      // Setup defaults for the texture
      OSG::ImagePtr img = OSG::Image::create();         // Temporary image for now
      OSG::UChar8 data[] = {0,0,0, 50,50,50, 100,100,100, 255,255,255};

      OSG::beginEditCP(img);
         img->set( OSG::Image::OSG_RGB_PF, 2, 2, 1, 1, 1, 0, data);
      OSG::endEditCP(img);

      // -- Setup texture and materials -- //
      mTextureChunk->setImage(img);
      mTextureChunk->setWrapS(GL_CLAMP);
      mTextureChunk->setWrapT(GL_CLAMP);
      mTextureChunk->setMagFilter(GL_LINEAR);
      mTextureChunk->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
      mTextureChunk->setEnvMode(GL_MODULATE);

      OSG::MaterialChunkPtr mat_chunk = OSG::MaterialChunk::create();
      OSG::beginEditCP(mat_chunk);
      {
         mat_chunk->setAmbient (OSG::Color4f(1.f, 1.f, 1.f, 1.f));
         mat_chunk->setDiffuse (OSG::Color4f(1.f, 1.f, 1.f, 1.f));
         mat_chunk->setEmission(OSG::Color4f(0.f, 0.f, 0.f, 1.f));
         mat_chunk->setSpecular(OSG::Color4f(0.f, 0.f, 0.f, 1.f));
         mat_chunk->setShininess(0);
         mat_chunk->setColorMaterial(GL_AMBIENT_AND_DIFFUSE);
      }
      OSG::endEditCP(mat_chunk);

      mBlendChunk->setSrcFactor(GL_SRC_ALPHA);
      mBlendChunk->setDestFactor(GL_ONE_MINUS_SRC_ALPHA);
      
      mTextMat->addChunk(mTextureChunk);
      mTextMat->addChunk(mBlendChunk);
      mTextMat->addChunk(mat_chunk);

      mTextGeom->setMaterial(mTextMat);
      mRootNode->setCore(mTextGeom);
   }

   void updateScore(int p1,int p2)
   {
	   static int counter=0;
      std::vector<std::string> lines;

      OSG::TextLayoutResult layout_result;
      OSG::TextLayoutParam layout_param;
      layout_param.maxExtend = mMaxExtent;
      layout_param.setLength(mMaxExtent);
      layout_param.spacing = mLineSpacing;

      //mFace->layout(lines, layout_param, layout_result);
      OSG::GeometryPtr geom_ptr = mTextGeom.get();

      OSG::Vec2f bounds = layout_result.textBounds;
      mFace->fillGeo(geom_ptr, layout_result, mGeoScale, mOffset, OSG::Color3f(0,1,0));


		char str[64];
		sprintf(str,"FOR: %d  AGAINST: %d",p1,p2);
         mFace->layout(str, layout_param, layout_result);
         mFace->addToGeom(geom_ptr, layout_result, mGeoScale, OSG::Vec3f(-4,-0.3,0.5), OSG::Color3f(1.0,0.0,0.0));
 
   }

   void updateFace()
   {
      // Try to create new face
       OSG::TextTXFParam param;
       param.textureWidth = mTextureWidth;
       param.gap = mTextGap;
       param.size = mFaceSize;

       OSG::TextTXFFace* new_face = OSG::TextTXFFace::create(mFamilyName, mStyle, param);
       if (NULL == new_face)
       {
          std::cerr << "ERROR: Failed to allocate face." << std::endl;
       }
       OSG::subRefP(mFace);
       mFace = new_face;
       OSG::addRefP(mFace);

       OSG::beginEditCP(mTextureChunk);
         OSG::ImagePtr face_image = mFace->getTexture();
         mTextureChunk->setImage(face_image);
       OSG::endEditCP(mTextureChunk);
   }

   /** Increment/decrement current texture size and regen face. */
   void incTextureSize(bool dec=false)
   {
      switch (mTextureWidth)
      {
      case 0:  mTextureWidth = (dec?0:16);  break;
      case 16:  mTextureWidth = (dec?0:32);  break;
      case 32:  mTextureWidth = (dec?16:64);  break;
      case 64:  mTextureWidth = (dec?32:128);  break;
      case 128:  mTextureWidth = (dec?64:256);  break;
      case 256:  mTextureWidth = (dec?128:512);  break;
      case 512:  mTextureWidth = (dec?256:1024);  break;
      case 1024:  mTextureWidth = (dec?512:1024);  break;
      }

      updateFace();
      updateScore(0,0);
   }

   void incFaceSize(bool inc=true)
   {
      if(inc)
         mFaceSize += 1;
      else if(0 != mFaceSize)
         mFaceSize -= 1;

      std::cout << "Set face size to: " << mFaceSize << std::endl;
      updateFace();
      updateScore(0,0);
   }

   /** Cycle to the next family in the set of available font families. */
   void goToNextFamily()
   {
      mFamilyName = mFamilies[mNextFamily];
      mNextFamily += 1;
      if(mNextFamily >= mFamilies.size())
      { mNextFamily = 0; }

     // std::cout << "New family: " << mFamilyName << std::endl;
      updateFace();
      updateScore(0,0);
   }

   void incLineSpacing(bool inc=true)
   {
      float increment(0.05f);
      if(!inc)
      { increment *= -1.0f; }

      mLineSpacing += increment;

      if(mLineSpacing < 0.0f)
      {  mLineSpacing = 0.0f; }

     // std::cout << "Set line spacing to: " << mLineSpacing << std::endl;
      updateScore(0,0);
   }

   void incMaxExtent(bool inc=true)
   {
      float increment(0.5f);
      if(!inc)
      { increment *= -1.0f; }

      mMaxExtent += increment;

      if(mMaxExtent < 0.0f)
      { mMaxExtent = 0.0f; }

    //  std::cout << "Set max extent to: " << mMaxExtent << std::endl;
       updateScore(0,0);
   }

   void incGeoScale(bool inc=true)
   {
      float increment(0.05f);
      if(!inc)
      { increment *= -1.0f; }

      mGeoScale += increment;

      if(mGeoScale < 0.0f)
      { mGeoScale = 0.0f; }

     // std::cout << "Set geo scale to: " << mGeoScale << std::endl;
      updateScore(0,0);
   }

   void incOffset(float xInc, float yInc)
   {
      mOffset[0] += xInc;
      mOffset[1] += yInc;
      std::cout << "Offset: " << mOffset << std::endl;
      updateScore(0,0);
   }


public:
   OSG::NodeRefPtr            mRootNode;     /**< Root node for text geom. */
   OSG::GeometryRefPtr        mTextGeom;     /**< Geom core for the text. */
   OSG::ChunkMaterialRefPtr   mTextMat;      /**< Material for the text geom. */
   OSG::TextureChunkRefPtr    mTextureChunk; /**< Texture chunk for the text material. */
   OSG::BlendChunkRefPtr      mBlendChunk;   /**< Blend chunk for the text material. */

   OSG::TextTXFFace*          mFace;
   std::string                mFamilyName;   /**< The name of the font family. */
   std::vector<std::string>   mFamilies;
   unsigned                   mNextFamily;   /**< Next text family to use. */
   OSG::TextFace::Style       mStyle;

   unsigned                   mTextGap;      /**< The gap between glyphs in pixels */
   unsigned                   mTextureWidth; /**< The width of the texture in pixels */
   unsigned                   mFaceSize;     /**< The "size" param of the face. */

   float                      mLineSpacing;  /**< Spacing to use in the layout. */
   float                      mMaxExtent;    /**< Maximum extent to use. */

   float                      mGeoScale;     /**< Scale for geometry. */
   OSG::Vec2f                 mOffset;       /**< Offset of text when building. */
};



