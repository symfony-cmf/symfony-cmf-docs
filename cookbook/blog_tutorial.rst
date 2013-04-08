Blog Tutorial
=============

This tutorial will show you how to create a simple blog application in the Symfony CMF
using the `BlogBundle`.

Getting started
---------------

Setting up the project
~~~~~~~~~~~~~~~~~~~~~~

First of all we need to create a new project. The easiest way to do this is to use composer
as follows:

.. code-block:: bash

    $ php composer.phar create-project --stability dev symfony-cmf/standard-edition /path/to/project

Replace ``path/to/project`` with the directory where you want your blog application to live.

Now lets add the blog bundle and the sension generator bundle to `composer.json`:

.. code-block:: javascript


    {
        "name": "symfony-cmf/standard-edition",
        [...]
        "require": {
            [...],
            "sensio/generator-bundle": "2.2.*",
            "symfony-cmf/blog-bundle": "dev-auto_route"
        },
        [...]
    }

Or alternatively:

.. code-block:: bash

    $ composer require sensio/generator-bundle "2.2.*"
    $ composer require symfony-cmf/blog-bundle "dev-auto_route"

And do a composer update:

.. code-block:: bash

    $ // ???

.. note:: 
    
    Remember that JSON requires the last item in a list to NOT have a trailing comma!

.. note::
    
    Why are we including the sension generator bundle? Well, I think it should be part
    of the standard distribution as it will make our lives easier later on.

Next add the blog bundle to ``AppKernel.php``::

    <?php

    // ./app/AppKernel.php

    class AppKernel
    {
        // ...
        $bundles = array(
            // ...
            new Symfony\Cmf\Bundle\BlogBundle\SymfonyCmfBlogBundle(),
            new Sensio\Bundle\GeneratorBundle\SensioGeneratorBundle(),
            // ...
    }

Now, if you try to invoke the console you should receieve an error:

.. code-block:: bash

    $ ./app/console
    
      [Symfony\Component\Config\Definition\Exception\InvalidConfigurationException]  
      The child node "blog_basepath" at path "symfony_cmf_blog" must be configured.  
                                                                                 
Great. So accordingly we now have to tell the blog bundle where in the PHPCR-PDM document
tree it should store the blog. Add the following to `config.yml`:

.. code-block:: yaml

    // ./app/config/config.yml
    symfony_cmf_blog:
        blog_basepath: /cms/content

Create the main bundle
~~~~~~~~~~~~~~~~~~~~~~

The main bundle is where all your application specific code will go, lets create it
using the sension generator bundle as follows::

.. code-block:: bash

    $ ./app/console generate:bundle --namespace=DTL\\BlogBundle --dir=src --format=annotation --no-interaction

This will create a new bundle called ``BlogBundle`` in ``src/DTL``, using annotations as the
configuration format. Replace DTL with your vendor name, this could be your companies name or 
your github username, or whatever you like.

.. note::

    Try running this command without any arguments to create the bundle interactively.

Fixtures
--------

Fixtures make developing an application so much easier. We create the fixtures using
the doctrine fixtures and the excellent faker library which can generate test data
for us.

The doctrine fixtures bundle is included in the standard distribution, meaning that
you already have it. The faker library can be installed as folllows:

.. code-block:: bash

    $ composer require fzaninotto/faker "dev-master"

.. note::

    The faker library is nice, but you can of course not include it and create the
    fixtures manually.

Now lets create the fixture file::

    <?php

    namespace DTL\MainBundle\DataFixtures\PHPCR;

    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\DataFixtures\OrderedFixtureInterface;
    use Symfony\Cmf\Bundle\BlogBundle\Document\Blog;
    use Symfony\Cmf\Bundle\BlogBundle\Document\Post;
    use Symfony\Component\DependencyInjection\ContainerAware;
    use PHPCR\Util\NodeHelper;

    class LoadBlogData extends ContainerAware implements FixtureInterface, OrderedFixtureInterface
    {
        public function getOrder()
        {
            return 10;
        }

        public function load(ObjectManager $dm)
        {
            $session = $dm->getPhpcrSession();

            NodeHelper::createPath($session, '/cms/content');

            $root = $dm->find(null, $basepath);

            $this->faker = \Faker\Factory::create();

            $blog = new Blog;
            $blog->setName('DTLs Blog');
            $blog->setParent($root);
            $dm->persist($blog);

            for ($i = 1; $i <= 20; $i++) {
                $p = new Post;
                $p->setTitle($this->faker->text(30));
                $p->setDate($this->faker->date);
                $p->setBody($this->faker->text(500));
                $p->setBlog($blog);
                $dm->persist($p);
            }

            $dm->flush();
        }
    }

Now lets load the fixtures:

.. code-block:: bash

    $ ./app/console doctrine:phpcr:load:fixtures

Oh dear that didn't seem to work!

.. code-block:: bash

   [PHPCR\RepositoryException]                                                                        
   SQLSTATE[HY000]: General error: 1 no such table: phpcr_workspaces 

We need to first initialize the database as follows:

.. code-block:: bash

    $ ./app/console doctrine:phpcr:init:dbal
    $ ./app/console doctrine:phpcr:register-system-node-types

The standard edition is preconfigured to work out-of-the-box with an sqlite
database which can be found in ``./app/app.sqlite``.

Have a look inside the database with the ``sqlite3`` command line tool:

.. code-block:: bash

    $ sqlite3 app/app.sqlite
    > .tables
    > .exit

OK. Now we can load the fixtures

.. code-block:: bash

    $ ./app/console doctrine:phpcr:load:fixtures

And inspect the contents of the database with a JCR-SQL2 query.

.. code-block:: bash

    $ ./app/console doctrine:phpcr:query "SELECT * FROM nt:unstructured WHERE phpcr:class=\"Symfony\Cmf\Bundle\BlogBundle\Document\Post\""
    Executing, language: JCR-SQL2
    Results:


    1. Row (Path: /cms/content/DTLs Blog/earum-quis-dolores-iste-quia, Score: 0):
           jcr:createdBy: NULL
           jcr:created: NULL
           jcr:primaryType: 'nt:unstructured'

    2. Row (Path: /cms/content/DTLs Blog/et-nulla-sit-molestiae-ipsum, Score: 0):
           jcr:createdBy: NULL
           jcr:created: NULL
           jcr:primaryType: 'nt:unstructured'

    [...]

