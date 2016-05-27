Getting Started
===============

Initializing the Project
------------------------

First, follow the generic steps in :doc:`../cookbook/database/create_new_project_phpcr_odm`
to create a new project using the PHPCR-ODM.

.. _gettingstarted_installadditionbundles:

Install Additional Bundles
~~~~~~~~~~~~~~~~~~~~~~~~~~

The complete tutorial requires the following packages:

* `symfony-cmf/routing-auto-bundle`_;
* `sonata-project/doctrine-phpcr-admin-bundle`_;
* `doctrine/data-fixtures`_;
* `symfony-cmf/menu-bundle`_.

Each part of the tutorial will detail the packages that it requires (if any) in a
section titled "installation".

If you intend to complete the entire tutorial you can save some time by adding
all of the required packages now:

.. code-block:: bash

    $ composer require symfony-cmf/routing-auto-bundle \
        symfony-cmf/menu-bundle \
        sonata-project/doctrine-phpcr-admin-bundle \
        symfony-cmf/tree-browser-bundle \
        doctrine/data-fixtures \
        symfony-cmf/routing-bundle

Initialize the Database
~~~~~~~~~~~~~~~~~~~~~~~

If you have followed the main instructions in :doc:`../bundles/phpcr_odm/introduction`
then you are using the `Doctrine DBAL Jackalope`_ PHPCR backend with MySQL and
you will need to create the database:

.. code-block:: bash

    $ php app/console doctrine:database:create

This will create a new database according to the configuration file
``parameters.yml``.

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

    $ php app/console generate:bundle --namespace=AppBundle --dir=src --format=yml --no-interaction

The Documents
.............

You will create two document classes, one for the pages and one for the posts.
These two documents share much of the same logic, so you create a ``trait``
to reduce code duplication::

    // src/AppBundle/Document/ContentTrait.php
    namespace AppBundle\Document;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

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
         * @PHPCR\Nodename()
         */
        protected $title;

        /**
         * @PHPCR\Field(type="string", nullable=true)
         */
        protected $content;

        protected $routes;

        public function getId()
        {
            return $this->id;
        }

        public function getParentDocument()
        {
            return $this->parent;
        }

        public function setParentDocument($parent)
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

    // src/AppBundle/Document/Page.php
    namespace AppBundle\Document;

    use Symfony\Cmf\Component\Routing\RouteReferrersReadInterface;

    use Doctrine\ODM\PHPCR\Mapping\Annotations as PHPCR;

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

    // src/AppBundle/Document/Post.php
    namespace AppBundle\Document;

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
:ref:`DynamicRouter to generate URLs <bundles-routing-dynamic-generator>` from
instances of these classes. (for example with ``{{ path(content) }}`` in Twig).

Repository Initializer
~~~~~~~~~~~~~~~~~~~~~~

:ref:`Repository initializers <phpcr-odm-repository-initializers>` enable you
to establish and maintain PHPCR nodes required by your application, for
example you will need the paths ``/cms/pages``, ``/cms/posts`` and
``/cms/routes``. The ``GenericInitializer`` class can be used easily
initialize a list of paths. Add the following to your service container
configuration:

.. configuration-block::

    .. code-block:: yaml

        # src/AppBundle/Resources/config/services.yml
        services:
            app.phpcr.initializer:
                class: Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer
                arguments:
                    - My custom initializer
                    - ["/cms/pages", "/cms/posts", "/cms/routes"]
                tags:
                    - { name: doctrine_phpcr.initializer }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <!-- src/AppBundle\Resources\services.xml -->
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:app="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://symfony.com/schema/dic/services
                http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- ... -->
            <services>
                <!-- ... -->

                <service id="app.phpcr.initializer"
                    class="Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer">

                    <argument>My custom initializer</argument>

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

        // src/AppBundle/Resources/config/services.php
        $container
            ->register(
                'app.phpcr.initializer',
                'Doctrine\Bundle\PHPCRBundle\Initializer\GenericInitializer'
            )
            ->addArgument('My custom initializer')
            ->addArgument(array('/cms/pages', '/cms/posts', '/cms/routes'))
            ->addTag('doctrine_phpcr.initializer')
        ;

.. note::

    The initializers operate at the PHPCR level, not the PHPCR-ODM level - this
    means that you are dealing with nodes and not documents. You do not have
    to understand these details right now. To learn more about PHPCR read
    :doc:`../cookbook/database/choosing_storage_layer`.

The initializers will be executed automatically when you load your data
fixtures (as detailed in the next section) or alternatively you can execute
them manually using the following command:

.. code-block:: bash

    $ php app/console doctrine:phpcr:repository:init

.. note::

    This command is `idempotent`_, which means that it is safe to run
    it multiple times, even when you have data in your repository. Note
    however that it is the responsibility of the initializer to respect
    idempotency!

You can check to see that the repository has been initialized by dumping the
content repository:

.. code-block:: bash

    $ php app/console doctrine:phpcr:node:dump

Create Data Fixtures
~~~~~~~~~~~~~~~~~~~~

You can use the doctrine data fixtures library to define some initial data for
your CMS.

Ensure that you have the following package installed:

.. code-block:: javascript

    {
        ...
        require: {
            ...
            "doctrine/data-fixtures": "1.0.*"
        },
        ...
    }

Create a page for your CMS::

    // src/AppBundle/DataFixtures/PHPCR/LoadPageData.php
    namespace AppBundle\DataFixtures\PHPCR;

    use AppBundle\Document\Page;
    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\ODM\PHPCR\DocumentManager;

    class LoadPageData implements FixtureInterface
    {
        public function load(ObjectManager $dm)
        {
            if (!$dm instanceof DocumentManager) {
                $class = get_class($dm);
                throw new \RuntimeException("Fixture requires a PHPCR ODM DocumentManager instance, instance of '$class' given.");
            }

            $parent = $dm->find(null, '/cms/pages');

            $page = new Page();
            $page->setTitle('Home');
            $page->setParentDocument($parent);
            $page->setContent(<<<HERE
    Welcome to the homepage of this really basic CMS.
    HERE
            );

            $dm->persist($page);
            $dm->flush();
        }
    }

and add some posts::

    // src/AppBundle/DataFixtures/PHPCR/LoadPostData.php
    namespace AppBundle\DataFixtures\PHPCR;

    use Doctrine\Common\DataFixtures\FixtureInterface;
    use Doctrine\Common\Persistence\ObjectManager;
    use Doctrine\ODM\PHPCR\DocumentManager;
    use AppBundle\Document\Post;

    class LoadPostData implements FixtureInterface
    {
        public function load(ObjectManager $dm)
        {
            if (!$dm instanceof DocumentManager) {
                $class = get_class($dm);
                throw new \RuntimeException("Fixture requires a PHPCR ODM DocumentManager instance, instance of '$class' given.");
            }

            $parent = $dm->find(null, '/cms/posts');

            foreach (array('First', 'Second', 'Third', 'Fourth') as $title) {
                $post = new Post();
                $post->setTitle(sprintf('My %s Post', $title));
                $post->setParentDocument($parent);
                $post->setContent(<<<HERE
    This is the content of my post.
    HERE
                );

                $dm->persist($post);
            }

            $dm->flush();
        }
    }

Then load the fixtures:

.. code-block:: bash

    $ php app/console doctrine:phpcr:fixtures:load

You should now have some data in your content repository.

.. _`idempotent`: https://en.wiktionary.org/wiki/idempotent
.. _`symfony-cmf/routing-auto-bundle`: https://packagist.org/packages/symfony-cmf/routing-auto-bundle
.. _`symfony-cmf/menu-bundle`: https://packagist.org/packages/symfony-cmf/menu-bundle
.. _`sonata-project/doctrine-phpcr-admin-bundle`: https://packagist.org/packages/sonata-project/doctrine-phpcr-admin-bundle
.. _`doctrine/data-fixtures`: https://packagist.org/packages/doctrine/data-fixtures
.. _`Doctrine DBAL Jackalope`: https://github.com/jackalope/jackalope-doctrine-dbal
.. _`Apache Jackrabbit`: https://jackrabbit.apache.org/jcr/index.html
.. _`pre persist lifecycle event`: http://docs.doctrine-project.org/projects/doctrine-phpcr-odm/en/latest/reference/events.html#lifecycle-callbacks
