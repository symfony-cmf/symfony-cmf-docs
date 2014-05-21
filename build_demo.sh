git config --global user.name "Sf Travis Bot"
git config --global user.email "sf-travis-bot@travis-ci.org"

git clone https://${GH_TOKEN}@github.com/WouterJ/symfony-cmf-docs-demo

if [[ ${TRAVIS_PULL_REQUEST} ]]; then
    DEMO_PREFIX=${TRAVIS_PULL_REQUEST}
else
    DEMO_PREFIX=${TRAVIS_BRANCH}
fi

DEMO_DIR_NAME=symfony-cmf-docs-demo/${DEMO_PREFIX}-${TRAVIS_BUILD_NUMBER}

rm -rf symfony-cmf-docs-demo/${DEMO_PREFIX}-*

mkdir $DEMO_DIR_NAME

cp -rf _build/html/* $DEMO_DIR_NAME

cd symfony-cmf-docs-demo

git add -A

git commit -m "Travis build ${TRAVIS_BUILD_NUMBER}"
git push origin

if [ $? != 0 ]; then
    echo "Error while pushing demo to GitHub"
else
    echo "Pushed demo to github, visit your demo at http://wouterj.github.com/symfony-cmf-docs-demo/${DEMO_DIR_NAME}"
fi
