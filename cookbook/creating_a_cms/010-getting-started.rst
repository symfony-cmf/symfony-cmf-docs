Part 1 - Getting Started
------------------------

Initializing the Project
~~~~~~~~~~~~~~~~~~~~~~~~

First, follow the generic steps in :doc:`create_new_project_phpcr_odm` to
create a new project using the PHPCR-ODM.

Install Additional Bundles
..........................

This tutorial requires the following packages:

* `symfony-cmf/routing-auto-bundle`_;
* `sonata-project/doctrine-phpcr-admin-bundle`_;
* `doctrine/data-fixtures`_;
* `symfony-cmf/menu-bundle`_.

Update ``composer.json`` to require them:

.. code-block:: javascript

    {
        ...
        require: {
            ...
            "symfony-cmf/routing-auto-bundle": "1.0.0@alpha",
            "symfony-cmf/menu-bundle": "1.0",
            "sonata-project/doctrine-phpcr-admin-bundle": "dev-master",
            "doctrine/data-fixtures": "1.0.0"
        },
        ...
    }

And add the packages to the kernel::

    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Symfony\Cmf\Bundle\RoutingBundle\CmfRoutingBundle(),
                new Symfony\Cmf\Bundle\RoutingAutoBundle\CmfRoutingAutoBundle(),
            );

            // ...
        }
    }

Initialize the Database
.......................

If you have followed the main instructions in
:doc:`create_new_project_phpcr_odm` then you are using the `Doctrine DBAL
Jackalope`_ PHPCR backend with MySQL and you will need to create the MySQL
database:

.. code-block:: bash

    $ mysqladmin create symfony -u root

This will create a new database called ``symfony`` - this is the name used by
default in the Symfony Standard Edition, change it as might be necessary.

The Doctrine DBAL backend needs to be initialized, the following command
will create the MySQL schema required to store the hierarchical
node content of the PHPCR content repository:

.. code-block:: bash

    $ php app/console doctrine:phpcr:init:dbal

.. note::

    The `Apache Jackrabbit`_ implementation is the reference java based
    backend and does not require such initialization. It does however require
    the use of Java.

Generate the Bundle
...................

Now you can generate the bundle in which you will write most of your code:

.. code-block:: bash

    $ php app/console generate:bundle --namespace=Acme/BasicCmsBundle --dir=src --no-interaction

The Documents
.............

You will create two document classes, one for the pages and one for the posts.
These two documents share much of the same logic, so you create a ``trait``
to reduce code duplication::

    // src/Acme/BasicCmsBundle/Document/ContentTrait.php
    namespace Acme\BasicCmsBundle\Document;

    trait ContentTrait
    {
        /**
         * @PHPCR\Id()
         */
        protected $id;

        /**
         * @PHPCR\ParentDocument()
         */
        protected $parent;

        /**
         * @PHPCR\NodeName()
         */
        protected $title;

        /**
         * @PHPCR\String(nullable=true)
         */
        protected $content;

        /**
         * @PHPCR\Referrers(
         *     referringDocument="Symfony\Cmf\Bundle\RoutingBundle\Doctrine\Phpcr\Route",
         *     referencedBy="content"
         * )
         */
        protected $routes;

        public function getId()
        {
            return $this->id;
        }

        public function getParent() 
        {
            return $this->parent;
        }
        
        public function setParent($parent)
        {
            $this->parent = $parent;
        }
        
        public function getTitle() 
        {
            return $this->title;
        }
        
        public function setTitle($title)
        {
            $this->title = $title;
        }

        public function getContent() 
        {
            return $this->content;
        }
        
        public function setContent($content)
        {
            $this->content = $content;
        }

        public function getRoutes()
        {
            return $this->routes;
        }
    }

.. note::

    Traits are only available as of PHP 5.4. If you are running a lesser
    version of PHP you may copy the above code into each class to have the
    same effect. You may not, however, ``extend`` one class from the other, as
    this will cause unintended behavior in the admin integration later on.

The ``Page`` class is therefore nice and simple::

    // src/Acme/BasicCmsBundle/Document/Page.php
    namespace Acme\BasicCmsBundle\Document;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;
    use Symfony\Cmf\Component\Routing\RouteReferrersReadInterface;

    /**
     * @PHPCR\Document(referenceable=true)
     */
    class Page implements RouteReferrersReadInterface
    {
        use ContentTrait;
    }

Note that the page document should be ``referenceable``. This will enable
other documents to hold a reference to the page. The ``Post`` class will also
be referenceable and in addition will automatically set the date using the
`pre persist lifecycle event`_ if it has not been explicitly set previously::

    // src/Acme/BasicCms/Document/Post.php
    namespace Acme\BasicCmsBundle\Document;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;
    use Symfony\Cmf\Component\Routing\RouteReferrersReadInterface;

    /**
     * @PHPCR\Document(referenceable=true)
     */
    class Post implements RouteReferrersReadInterface
    {
        use ContentTrait;

        /**
         * @PHPCR\Date()
         */
        protected $date;

        /**
         * @PHPCR\PrePersist()
         */
        public function updateDate()
        {
            if (!$this->date) {
                $this->date = new \DateTime();
            }
        }

        public function getDate()
        {
            return $this->date;
        }

        public function setDate(\DateTime $date)
        {
            $this->date = $date;
        }
    }

Both the ``Post`` and ``Page`` classes implement the
``RouteReferrersReadInterface``. This interface enables the 
`DynamicRouter to generate URLs`_ from instances of these classes. (for
example with ``{{ path(content) }}`` in Twig).

Repository Initializer
~~~~~~~~~~~~~~~~~~~~~~

:ref:`Repository initializers <../../bundles/phpcr_odm>` enable you to establish and maintain PHPCR nodes
required by your application, for example you will need the paths
``/cms/pages``, ``/cms/posts`` and ``/cms/routes``. The ``GenericInitializer``
class can be used easily initialize a list of paths. Add the following to your
service container configuration:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BasicCmsBundle/Resources/config/services.yml
        services:
            acme.basic_cms.phpcr.initializer:
                class: Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer
                arguments: 
                    - ["/cms/pages", "/cms/posts", "/cms/routes"]
                tags:
                    - { name: doctrine_phpcr.initializer }

    .. code-block:: xml

        <!-- src/Acme\BasicCmsBundle\Resources\services.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_demo="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://symfony.com/schema/dic/services 
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- ... -->
            <services>
                <!-- ... -->

                <service id="acme.basic_cms.phpcr.initializer"
                    class="Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer">

                    <argument type="collection">
                        <argument>/cms/pages</argument>
                        <argument>/cms/posts</argument>
                        <argument>/cms/routes</argument>
                    </argument>

                    <tag name="doctrine_phpcr.initializer"/>
                </service>
            </services>
        </container>

    .. code-block:: php

        // src/Acme/BasicCmsBundle/Resources/config/services.php
        $container
            ->register(
                'acme.basic_cms.phpcr.initializer',
                'Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer'
            )
            ->addArgument(array('/cms/pages', '/cms/posts', '/cms/routes'))
            ->addTag('doctrine_phpcr.initializer')
        ;

.. note::

    The initializers operate at the PHPCR level, not the PHPCR-ODM level - this
    means that you are dealing with nodes and not documents.

Execute the ``doctrine:phpcr:repository:init`` command to initialize (or
reinitialize) the repository:

.. code-block:: bash

    $ php app/console doctrine:phpcr:repository:init

.. note::

    This command is `idempotent`_, which means that it is safe to run
    it multiple times, even when you have data in your repository. Note
    however that it is the responsibility of the initializer to respect
    idempotency!

Create Data Fixtures
~~~~~~~~~~~~~~~~~~~~

Create a page for your CMS::

    // src/Acme/BasicCmsBundle/DataFixtures/PHPCR/LoadPageData.php
    namespace Acme\BasicCmsBundle\DataFixtures\PHPCR;

    use Acme\BasicCmsBundle\Document\Page;
    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use PHPCR\Util\NodeHelpen;

    class LoadPageData implements FixtureInterface
    {
        public function load(ObjectManager $dm)
        {
            $parent = $dm->find(null, '/cms/pages');

            $page = new Page();
            $page->setTitle
            $page->setParent($parent);
            $page->setContent(<<<HERE
    Welcome to the homepage of this really basic CMS.
    HERE
            );

            $dm->persist($page);
            $dm->flush();
        }
    }

and add some posts::

    // src/Acme/BasicCmsBundle/DataFixtures/PHPCR/LoadPostData.php
    namespace Acme\BasicCmsBundle\DataFixtures\Phpcr;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Acme\BasicCmsBundle\Document\Post;
    use PHPCR\Util\NodeHelper;

    class LoadPostData implements FixtureInterface
    {
        public function load(ObjectManager $dm)
        {
            $parent = $dm->find(null, '/cms/posts');

            foreach (array('First', 'Second', 'Third', 'Forth') as $title) {
                $post = new Post();
                $post->setTitle(sprintf('My %s Post', $title));
                $post->setParent($parent);
                $post->setContent(<<<HERE
    This is the content of my post.
    HERE
                );

                $dm->persist($post);
            }

            $dm->flush();
        }
    }

and load the fixtures:

.. code-block:: bash

    $ php app/console doctrine:phpcr:fixtures:load

You should now have some data in your content repository.

.. _`routingautobundle documentation`: http://symfony.com/doc/current/cmf/bundles/routing_auto.html
.. _`dynamicrouter to generate urls`: http://symfony.com/doc/current/cmf/bundles/routing/dynamic.html#url-generation-with-the-dynamicrouterA
.. _`idempotent`: http://en.wiktionary.org/wiki/idempotent
.. _`symfony-cmf/routing-auto-bundle`: https://packagist.org/packages/symfony-cmf/routing-auto-bundle
.. _`symfony-cmf/menu-bundle`: https://packagist.org/packages/symfony-cmf/menu-bundle
.. _`sonata-project/doctrine-phpcr-admin-bundle`: https://packagist.org/packages/sonata-project/doctrine-phpcr-admin-bundle
.. _`doctrine/data-fixtures`: https://packagist.org/packages/doctrine/data-fixtures
.. _`doctrine dbal jackalope`: https://github.com/jackalope/jackalope-doctrine-dbal
