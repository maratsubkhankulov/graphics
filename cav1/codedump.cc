bones[1].getRestTransformMatrix() = bones[0].getRestTransformMatrix()*bones[1].getRotationMatrix()*bones[1].getTranslationMatrix();
bones[2].getRestTransformMatrix() = bones[1].getRestTransformMatrix()*bones[2].getRotationMatrix()*bones[2].getTranslationMatrix();
bones[3].getRestTransformMatrix() = bones[2].getRestTransformMatrix()*bones[3].getRotationMatrix()*bones[3].getTranslationMatrix();
bones[4].getRestTransformMatrix() = bones[3].getRestTransformMatrix()*bones[4].getRotationMatrix()*bones[4].getTranslationMatrix();

bones[5].getRestTransformMatrix() = bones[0].getRestTransformMatrix()*bones[5].getRotationMatrix()*bones[5].getTranslationMatrix();
bones[6].getRestTransformMatrix() = bones[5].getRestTransformMatrix()*bones[6].getRotationMatrix()*bones[6].getTranslationMatrix();
bones[7].getRestTransformMatrix() = bones[6].getRestTransformMatrix()*bones[7].getRotationMatrix()*bones[7].getTranslationMatrix();

bones[8].getRestTransformMatrix() = bones[6].getRestTransformMatrix()*bones[8].getRotationMatrix()*bones[8].getTranslationMatrix();
bones[9].getRestTransformMatrix() = bones[8].getRestTransformMatrix()*bones[9].getRotationMatrix()*bones[9].getTranslationMatrix();
bones[10].getRestTransformMatrix() = bones[9].getRestTransformMatrix()*bones[10].getRotationMatrix()*bones[10].getTranslationMatrix();
bones[11].getRestTransformMatrix() = bones[10].getRestTransformMatrix()*bones[11].getRotationMatrix()*bones[11].getTranslationMatrix();
bones[12].getRestTransformMatrix() = bones[11].getRestTransformMatrix()*bones[12].getRotationMatrix()*bones[12].getTranslationMatrix();

bones[13].getRestTransformMatrix() = bones[6].getRestTransformMatrix()*bones[13].getRotationMatrix()*bones[13].getTranslationMatrix();
bones[14].getRestTransformMatrix() = bones[13].getRestTransformMatrix()*bones[9].getRotationMatrix()*bones[9].getTranslationMatrix();
bones[15].getRestTransformMatrix() = bones[9].getRestTransformMatrix()*bones[10].getRotationMatrix()*bones[10].getTranslationMatrix();
bones[16].getRestTransformMatrix() = bones[10].getRestTransformMatrix()*bones[11].getRotationMatrix()*bones[11].getTranslationMatrix();
bones[17].getRestTransformMatrix() = bones[11].getRestTransformMatrix()*bones[12].getRotationMatrix()*bones[12].getTranslationMatrix();	*/
