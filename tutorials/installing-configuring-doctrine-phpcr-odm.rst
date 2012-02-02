Installing and configuring Doctrine PHPCR ODM
=============================================

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

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``deps`` file::

    [DoctrinePHPCRBundle]
    git=http://github.com/doctrine/DoctrinePHPCRBundle.git
    target=/bundles/Doctrine/Bundle/PHPCRBundle
    
    [symfony-cmf]
    git=http://github.com/symfony-cmf/symfony-cmf.git
    git_command=submodule update --init --recursive

Register namespaces
~~~~~~~~~~~~~~~~~~~
Next step is to add the autoloader entries in ``app/autoload.php``::

    $loader->registerNamespaces(array(
        // ...

        'Doctrine\\Bundle'                      => __DIR__.'/../vendor/bundles',
        'Doctrine\\ODM\\PHPCR'                  => __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib',
        'Doctrine\\Common'                      => __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/vendor/doctrine-common/lib',
        'Jackalope'                             => __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/vendor/jackalope/src',
        'PHPCR'                                 => array(
                                                     __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/vendor/jackalope/lib/phpcr/src',
                                                     __DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/vendor/jackalope/lib/phpcr-utils/src'
                                                   ),
        // ...
    ));
    
Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method::

    public function registerBundles()
    {
        $bundles = array(
            // ...

            // Doctrine PHPCR
            new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),

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


Registering system node types
----------------------------
PHPCR ODM uses a `custom node type <https://github.com/doctrine/phpcr-odm/wiki/Custom-node-type-phpcr%3Amanaged>`_ to track meta information without interfering with your content. There is a command that makes it trivial to register this type and the phpcr namespace::

    php app/console doctrine:phpcr:register-system-node-types