SonataDoctrinePhpcrAdminBundle
==============================

The `SonataDoctrinePhpcrAdminBundle <https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle#readme>`_
provides integration with the SonataAdminBundle to enable easy creation of admin UIs.

.. index:: TreeBundle

Dependencies
------------

* `SonataAdminBundle <https://github.com/sonata-project/SonataAdminBundle>`_
* `TreeBundle <https://github.com/symfony-cmf/TreeBundle#readme>`_

Configuration
-------------

.. configuration-block::

    .. code-block:: yaml

        sonata_doctrine_phpcr_admin:
            templates:
                form:

                    # Default:
                    - SonataDoctrinePHPCRAdminBundle:Form:form_admin_fields.html.twig
                filter:

                    # Default:
                    - SonataDoctrinePHPCRAdminBundle:Form:filter_admin_fields.html.twig
                types:
                    list:

                        # Prototype
                        name:                 []
            document_tree:
                # Prototype
                class: # name of the class
                    # class names of valid children, manage tree operations for them and hide other children
                    valid_children:       []
                    image: