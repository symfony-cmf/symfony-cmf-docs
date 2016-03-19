.. index::
    single: PHPCR-ODM, Creating a New Project

Create a New Project with PHPCR-ODM
===================================

This article will show you how to create a new Symfony project from the
`Symfony Standard Edition`_ using PHPCR-ODM instead of (or in addition to) the
`Doctrine ORM`_.

It is assumed that you have `installed composer`_.

.. note::

    This walkthrough is intended to get you off the ground quickly, for more
    detailed documentation on integrating the PHPCR-ODM bundle see 
    the documentation for the :doc:`../../bundles/phpcr_odm/index`.

General Instructions using Jackalope Doctrine DBAL
--------------------------------------------------

The `Jackalope`_ Doctrine DBAL backend will use `Doctrine DBAL`_ to store the
content repository.

**Step 1**: Create a new Symfony project with composer based on the standard edition:

.. code-block:: bash

    $ composer create-project symfony/framework-standard-edition <your project name>/ --no-install

**Step 2**: Add the required packages to ``composer.json``:

.. code-block:: javascript

    {
        ...
        "require": {
            ...
            "doctrine/phpcr-bundle": "~1.2",
            "doctrine/phpcr-odm": "~1.2",
            "jackalope/jackalope-doctrine-dbal": "~1.1,>=1.1.2"
        }
    }



**Step 3**: (*optional*) Remove the Doctrine ORM:

* Remove the ``doctrine\orm`` package from ``composer.json``;
* Remove the ``orm`` section from ``app/config/config.yml``.

**Step 4**: Add the DoctrinePHPCRBundle to the ``AppKernel``::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Doctrine\Bundle\PHPCRBundle\DoctrinePHPCRBundle(),
            );

            // ...
        }
    }

**Step 5**: Modify ``parameters.yml.dist``, adding the required PHPCR-ODM settings:

.. code-block:: yaml

    # app/config/parameters.yml.dist
    parameters:
        # ...
        phpcr_backend:
            type: doctrinedbal
            connection: default
        phpcr_workspace: default
        phpcr_user: admin
        phpcr_pass: admin

.. note::

    You are modifying ``parameters.yml.dist`` and not ``paramaters.yml``.
    This is because the Standard Edition will use this file as a template when
    updating the configuration.

**Step 6**: Add the Doctrine PHPCR configuration to the main application configuration:

.. configuration-block::

    .. code-block:: yaml

        # ...
        doctrine_phpcr:
           # configure the PHPCR session
           session:
               backend: "%phpcr_backend%"
               workspace: "%phpcr_workspace%"
               username: "%phpcr_user%"
               password: "%phpcr_pass%"
           # enable the ODM layer
           odm:
               auto_mapping: true
               auto_generate_proxy_classes: "%kernel.debug%"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">
                <session backend="%phpcr_backend%"
                    workspace="%phpcr_workspace%"
                    username="%phpcr_user%"
                    password="%phpcr_pass%"
                />

                <odm auto-mapping="true"
                    auto-generate-proxy-classes="%kernel.debug%"
                />
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'backend' => '%phpcr_backend%',
                'workspace' => '%phpcr_workspace%',
                'username' => '%phpcr_username%',
                'password' => '%phpcr_password%',
            ),
            'odm' => array(
                'auto_mapping' => true,
                'auto_generate_proxy_classes' => '%kernel.debug%',
            ),
        ));

**Step 7**: Run ``composer update``:

.. code-block:: bash

    $ composer update

After installing the packages composer will ask you to confirm or modify the
default parameters defined in ``parameters.yml.dist`` and then generate the
``parameters.yml`` file.

Your should now be all set to start using PHPCR-ODM in your project!

.. _`Symfony Standard Edition`: https://github.com/symfony/symfony-standard
.. _`Doctrine ORM`: https://github.com/doctrine/doctrine2
.. _`Apache Jackrabbit`: https://jackrabbit.apache.org
.. _`Jackalope`: https://github.com/jackalope/jackalope
.. _`Doctrine DBAL`: https://github.com/doctrine/dbal
.. _`installed composer`: https://getcomposer.org/doc/00-intro.md#system-requirements
