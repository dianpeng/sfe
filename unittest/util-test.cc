#include <include/util.h>
#include <gtest/gtest.h>

namespace sfe {
namespace util{

TEST(Util,ParseBlendMode) {
  {
    sf::BlendMode output;
    ASSERT_TRUE(ParseBlendMode(".src-color=zero;.dest-alpha=one-minus-dest-alpha;"
                               ".dest-color=one;.src-alpha=src-alpha;"
                               ".color-equation=add;.alpha-equation=sub;",&output));
    ASSERT_EQ(sf::BlendMode::Zero,output.colorSrcFactor);
    ASSERT_EQ(sf::BlendMode::One ,output.colorDstFactor);
    ASSERT_EQ(sf::BlendMode::Add ,output.colorEquation);
    ASSERT_EQ(sf::BlendMode::OneMinusDstAlpha,output.alphaDstFactor);
    ASSERT_EQ(sf::BlendMode::SrcAlpha,output.alphaSrcFactor);
    ASSERT_EQ(sf::BlendMode::Subtract,output.alphaEquation);
  }
}

} // namespace util
} // namespace sfe

int main( int argc, char* argv[] ) {
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
