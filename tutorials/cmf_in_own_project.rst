

Setting up the CMF bundles in your own project
==============================================
The Symfony CMF project makes it easier for developers to add CMS functionality to applications built with the Symfony2 PHP framework. Key development principles for the provided set of bundles are scalability, usability, documentation and testing.

The goal of this tutorial is to create an Symfony2 application using all the CMF bundles together. It is good to know that these CMF bundles can be used independently too.

If this is your first encounter with the Symfony CMF it would be a good idea to first take a look at `the big picture <http://slides.liip.ch/static/2012-01-17_symfony_cmf_big_picture.html#1>`_ and/or the `CMF sandbox environment <https://github.com/symfony-cmf/symfony-cmf>`_ which is a pre-installed Symfony / CMF application containing all CMF components.

Setting up a content repository
-------------------------------
As a backend content repository we're going to install Apache Jackrabbit:

- Make sure you have Java Virtual Machine installed on your box. If not, you can grab one from here: http://www.java.com/en/download/manual.jsp
- Download the latest version from the `Jackrabbit Downloads page <http://jackrabbit.apache.org/downloads.html>`_
- Run the server. Go to the folder where you downloaded the .jar file and launch it::

    java -jar jackrabbit-standalone-*.jar

Going to http://localhost:8080/ should now display a Apache Jackrabbit page.

More information about `running a Jackrabbit server <https://github.com/jackalope/jackalope/wiki/Running-a-jackrabbit-server>`_
can be found on the Jackalope wiki.

As we are using Jackalope as our PHPCR implementation we could also chose other storage backends like relational databases but for this tutorial we're going to use Jackrabbit.

Installation of the CMF
-----------------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Download the CMF bundles (and depencies) to the vendor folder. Add the following to your ``deps`` file::

    [DoctrinePHPCRBundle]
    git=http://github.com/doctrine/DoctrinePHPCRBundle.git
    target=/bundles/Doctrine/Bundle/PHPCRBundle

    [symfony-cmf]
        git=http://github.com/symfony-cmf/symfony-cmf.git
        git_command=submodule update --init --recursive

    [ChainRoutingBundle]
        git=git://github.com/symfony-cmf/ChainRoutingBundle.git
        target=/bundles/Symfony/Cmf/Bundle/ChainRoutingBundle

    [knp-menu]
        git=https://github.com/KnpLabs/KnpMenu.git

    [KnpMenuBundle]
        git=http://github.com/KnpLabs/KnpMenuBundle.git
        target=/bundles/Knp/Bundle/MenuBundle

    [SonatajQueryBundle]
        git=http://github.com/sonata-project/SonatajQueryBundle.git
        target=/bundles/Sonata/jQueryBundle

    [SonataAdminBundle]
        git=https://github.com/sonata-project/SonataAdminBundle.git
        target=/bundles/Sonata/AdminBundle

    [SonataDoctrinePHPCRAdminBundle]
        git=https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle.git
        target=/bundles/Sonata/DoctrinePHPCRAdminBundle

    [TreeBundle]
        git=git://github.com/symfony-cmf/TreeBundle.git
        target=/bundles/Symfony/Cmf/Bundle/TreeBundle


And run the vendors script to download the bundles::

    php bin/vendors install

After every vendor install/update make sure to run the following command in the symfony-cmf folder ``vendor/symfony-cmf/``::

    git submodule update --recursive --init


Register namespaces
~~~~~~~~~~~~~~~~~~~
Next step is to add the autoloader entries in ``app/autoload.php``::

    $loader->registerNamespaces(array(
        // ...

        // CMF
        'Symfony\\Cmf'                          => array(__DIR__.'/../vendor/symfony-cmf/src', __DIR__.'/../vendor/bundles'),
        'Doctrine\\Bundle'                      => __DIR__.'/../vendor/bundles',
        'Doctrine\\ODM\\PHPCR'                  => __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib',
        'Doctrine\\Common'                      => __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/vendor/doctrine-common/lib',
        'Jackalope'                             => __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/vendor/jackalope/src',
        'PHPCR'                                 => array(
                                                     __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/vendor/jackalope/lib/phpcr/src',
                                                     __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/vendor/jackalope/lib/phpcr-utils/src'
                                                   ),
        // Menu bundle
        'Knp\\Menu'        => __DIR__.'/../vendor/knp-menu/src',
        'Knp\\Bundle'      => __DIR__.'/../vendor/bundles',

        // Sonata
        'Sonata'           => __DIR__.'/../vendor/bundles',

        // ...
    ));                                              .

Register annotations
~~~~~~~~~~~~~~~~~~~~
Add autoloader entries in ``app/autoload.php`` for the ODM annotations right after the last ``AnnotationRegistry::registerFile`` line::

    // ...
    AnnotationRegistry::registerFile(__DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');
    // ...

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method::

    public function registerBundles()
    {
        $bundles = array(
            // ...

            // Doctrine PHPCR
            new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),

            // CMF stuff
            new Symfony\Cmf\Bundle\ChainRoutingBundle\SymfonyCmfChainRoutingBundle(),
            new Symfony\Cmf\Bundle\CoreBundle\SymfonyCmfCoreBundle(),
            new Symfony\Cmf\Bundle\MultilangContentBundle\SymfonyCmfMultilangContentBundle(),
            new Symfony\Cmf\Bundle\ContentBundle\SymfonyCmfContentBundle(),
            new Symfony\Cmf\Bundle\PHPCRBrowserBundle\SymfonyCmfPHPCRBrowserBundle(),

            // Menu stuff
            new Knp\Bundle\MenuBundle\KnpMenuBundle(),
            new Symfony\Cmf\Bundle\MenuBundle\SymfonyCmfMenuBundle(),

            // Admin stuff
            new Symfony\Cmf\Bundle\TreeBundle\SymfonyCmfTreeBundle(),
            new Sonata\jQueryBundle\SonatajQueryBundle(),
            new Sonata\AdminBundle\SonataAdminBundle(),
            new Sonata\DoctrinePHPCRAdminBundle\SonataDoctrinePHPCRAdminBundle(),

        );
        // ...
    }

Configuration
-------------
Next step is to configure the bundles.

Doctrine PHPCR ODM
~~~~~~~~~~~~~~~~~~
Basic configuration, add to ``app/config/config.yml``::

    doctrine_phpcr:
        session:
            backend:
                type: jackrabbit
                # replace localhost if you're not running the server locally
                url: http://localhost:8080/server/
            workspace: default
            username: admin
            password: admin

More information on configuring this bundle can be found `here <https://github.com/doctrine/DoctrinePHPCRBundle#readme>`_.

SymfonyCmfChainRoutingBundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Basic configuration, add to ``app/config/config.yml``::

    symfony_cmf_chain_routing:
        chain:
            routers_by_id:
                symfony_cmf_chain_routing.doctrine_router: 200
                router.default: 100
        doctrine:
            enabled: true

TODO: link to reference

SonataAdminBundle
~~~~~~~~~~~~~~~~~
Add route in ``app/config/routing.yml`` ::

    admin:
        resource: '@SonataAdminBundle/Resources/config/routing/sonata_admin.xml'
        prefix: /admin

TODO: link to reference

Registering system node types
----------------------------
PHPCR ODM uses a `custom node type <https://github.com/doctrine/phpcr-odm/wiki/Custom-node-type-phpcr%3Amanaged>`_ to track meta information without interfering with your content. There is a command that makes it trivial to register this type and the phpcr namespace::

    php app/console doctrine:phpcr:register-system-node-types
