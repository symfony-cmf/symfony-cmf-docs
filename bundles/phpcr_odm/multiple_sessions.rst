.. index::
    single: Multisession; DoctrinePHPCRBundle

Configuring multiple sessions for PHPCR-ODM
===========================================

If you need more than one PHPCR backend, you can define ``sessions`` as child
of the ``session`` information. Each session has a name and the configuration
following the same schema as what is directly in ``session``. You can also
overwrite which session to use as ``default_session``.

.. _bundle-phpcr-odm-multiple-phpcr-sessions:

Multiple PHPCR Sessions
-----------------------

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        doctrine_phpcr:
            session:
                default_session: ~
                sessions:
                    <name>:
                        workspace: ... # Required
                        username:  ~
                        password:  ~
                        backend:
                            # ...
                        options:
                            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">
                <session default-session="null">
                    <!-- workspace: Required -->
                    <session name="<name>"
                        workspace="..."
                        username="null"
                        password="null"
                    >
                        <backend>
                            <!-- ... -->
                        </backend>
                        <option>
                            <!-- ... -->
                        </option>
                    </session>
                </session>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'default_session' => null,
                'sessions' => array(
                    '<name>' => array(
                        'workspace' => '...', // Required
                        'username'  => null,
                        'password'  => null,
                        'backend'   => array(
                            // ...
                        ),
                        'options'   => array(
                            // ...
                        ),
                    ),
                ),
            ),
        ));

Multiple Document Managers
--------------------------

If you are using the ODM, you will also want to configure multiple document
managers.

Inside the odm section, you can add named entries in the
``document_managers``. To use the non-default session, specify the session
attribute.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        odm:
            default_document_manager: ~
            document_managers:
                <name>:
                    session: <sessionname>
                    # ... configuration as above

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">
                <odm default-document-manager="null">
                    <document-manager
                        name="<name>"
                        session="<sessionname>"
                    >
                        <!-- ... configuration as above -->
                    </document-manager>
                </odm>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'odm' => array(
                'default_document_manager' => null,
                'document_managers' => array(
                    '<name>' => array(
                        'session' => '<sessionname>',
                        // ... configuration as above
                    ),
                ),
            ),
        ));

Bringing it all together
------------------------

A full example looks as follows:

.. configuration-block::

    .. code-block:: yaml

        doctrine_phpcr:
            # configure the PHPCR sessions
            session:
                sessions:
                    default:
                        backend: "%phpcr_backend%"
                        workspace: "%phpcr_workspace%"
                        username: "%phpcr_user%"
                        password: "%phpcr_pass%"

                    website:
                        backend:
                            type: jackrabbit
                            url: "%magnolia_url%"
                        workspace: website
                        username: "%magnolia_user%"
                        password: "%magnolia_pass%"

                    dms:
                        backend:
                            type: jackrabbit
                            url: "%magnolia_url%"
                        workspace: dms
                        username: "%magnolia_user%"
                        password: "%magnolia_pass%"

            # enable the ODM layer
            odm:
                auto_generate_proxy_classes: "%kernel.debug%"
                document_managers:
                    default:
                        session: default
                        mappings:
                            SandboxMainBundle: ~
                            CmfContentBundle: ~
                            CmfMenuBundle: ~
                            CmfRoutingBundle: ~

                    website:
                        session: website
                        configuration_id: sandbox_magnolia.odm_configuration
                        mappings:
                            SandboxMagnoliaBundle: ~

                    dms:
                        session: dms
                        configuration_id: sandbox_magnolia.odm_configuration
                        mappings:
                            SandboxMagnoliaBundle: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://doctrine-project.org/schema/symfony-dic/odm/phpcr">
                <session>
                    <session name="default"
                        backend="%phpcr_backend%"
                        workspace="%phpcr_workspace%"
                        username="%phpcr_user%"
                        password="%phpcr_pass%"
                    />
                    <session name="website"
                        workspace="website"
                        username="%magnolia_user%"
                        password="%magnolia_pass%"
                    >
                        <backend type="jackrabbit" url="%magnolia_url%"/>
                    </session>
                    <session name="dms"
                        workspace="dms"
                        username="%magnolia_user%"
                        password="%magnolia_pass%"
                    >
                        <backend type="jackrabbit" url="%magnolia_url%"/>
                    </session>
                </session>

                <!-- enable the ODM layer -->
                <odm auto-generate-proxy-classes="%kernel.debug%">
                    <document-manager
                        name="default"
                        session="default"
                    >
                        <mapping name="SandboxMainBundle" />
                        <mapping name="CmfContentBundle" />
                        <mapping name="CmfMenuBundle" />
                        <mapping name="CmfRoutingBundle" />
                    </document-manager>

                    <document-manager
                        name="website"
                        session="website"
                        configuration-id="sandbox_magnolia.odm_configuration"
                    >
                        <mapping name="SandboxMagnoliaBundle" />
                    </document-manager>

                    <document-manager
                        name="dms"
                        session="dms"
                        configuration-id="sandbox_magnolia.odm_configuration"
                    >
                        <mapping name="SandboxMagnoliaBundle" />
                    </document-manager>

                </odm>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('doctrine_phpcr', array(
            'session' => array(
                'sessions' => array(
                    'default' => array(
                        'backend'   => '%phpcr_backend%',
                        'workspace' => '%phpcr_workspace%',
                        'username'  => '%phpcr_user%',
                        'password'  => '%phpcr_pass%',
                    ),
                    'website' => array(
                        'backend' => array(
                            'type' => 'jackrabbit',
                            'url'  => '%magnolia_url%',
                        ),
                        'workspace' => 'website',
                        'username'  => '%magnolia_user%',
                        'password'  => '%magnolia_pass%',
                    ),
                    'dms' => array(
                        'backend' => array(
                            'type' => 'jackrabbit',
                            'url'  => '%magnolia_url%',
                        ),
                        'workspace' => 'dms',
                        'username'  => '%magnolia_user%',
                        'password'  => '%magnolia_pass%',
                    ),
                ),
            ),

            // enable the ODM layer
            'odm' => array(
                'auto_generate_proxy_classes' => '%kernel.debug%',
                'document_managers' => array(
                    'default' => array(
                        'session'  => 'default',
                        'mappings' => array(
                            'SandboxMainBundle' => null,
                            'CmfContentBundle'  => null,
                            'CmfMenuBundle'     => null,
                            'CmfRoutingBundle'  => null,
                        ),
                    ),
                    'website' => array(
                        'session'          => 'website',
                        'configuration_id' => 'sandbox_magnolia.odm_configuration',
                        'mappings'         => array(
                            'SandboxMagnoliaBundle' => null,
                        ),
                    ),
                    'dms' => array(
                        'session'          => 'dms',
                        'configuration_id' => 'sandbox_magnolia.odm_configuration',
                        'mappings'         => array(
                            'SandboxMagnoliaBundle' => null,
                        ),
                    ),
                ),
            ),
        ));


You can access the managers through the manager registry available in
``doctrine_phpcr``::

    /** @var $container \Symfony\Component\DependencyInjection\ContainerInterface */

    // get the named manager from the registry
    $dm = $container->get('doctrine_phpcr')->getManager('website');

    // get the manager for a specific document class
    $dm = $container->get('doctrine_phpcr')->getManagerForClass('CmfContentBundle:StaticContent');

Additionally, each manager is available as a service in the DI container.
The service name is ``doctrine_phpcr.odm.<name>_document_manager`` so for
example the website manager is called
``doctrine_phpcr.odm.website_document_manager``.
