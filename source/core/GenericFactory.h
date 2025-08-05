/**
  @file GenericFactory.h
  This files defines the GenericFactory template, and the helper class RegisterInFactory.
*/

/**
   @defgroup factory Factory
   The factory instantiates and registers different classes to produce, and consume them in the system.
*/

#ifndef GENERIC_FACTORY_H
#define GENERIC_FACTORY_H

#include <cv.h>
#include <map>
#include <string>

/**
  A generic factory that receives the type to create and the key to create it.
  It registers the class ManufacturedType using the ClassIDKey as key through the RegisterInFactory helper class. 
  The GenericFactory is used as a singleton. Thus, its constructors are private and you should access it through the GenericFactory::instance function.
  To use this class you should first register the objects, and then
  @code
    GenericFactory<Base>::instance().create("DerivedClassKey");
  @endcode

  @tparam ManufacturedType is the type of the class to create.
  @tparam ClassIDKey is the type of the key used to create the ManufacturedType class. By default it uses a std::string, but it can be any datatype.
  @warning Note that all the objects created by the GenericFactory must have a default constructor, and any further initialization should be done later on.  
  @ingroup factory
  @see RegisterInFactory
*/
template <class ManufacturedType, typename ClassIDKey=std::string>
class GenericFactory {

  // Useful types
  typedef cv::Ptr<ManufacturedType> (*BaseCreateFn)();
  typedef typename std::map<ClassIDKey, BaseCreateFn> FnRegistry;
  /// Creator functions registry
  FnRegistry registry;
  // Stop constructors
  GenericFactory(){}
  GenericFactory(const GenericFactory&);            // Not implemented
  GenericFactory &operator=(const GenericFactory&); // Not implemented

public:
  /** This function constructs the instance of the object defined by the className key.
    @param className is the identifier of the object to create.
    @return the instance of the created object.
  */
  cv::Ptr<ManufacturedType> create(const ClassIDKey &className) const {
    cv::Ptr<ManufacturedType> theObject(0);
    typename FnRegistry::const_iterator regEntry = registry.find(className);
    if (regEntry != registry.end()) {
        theObject = regEntry->second();
    }
    return theObject;
  }

  /** Static constructor. 
    @return the static instance of the factory.
  */
  static GenericFactory<ManufacturedType, ClassIDKey> &instance(){ 
    static GenericFactory<ManufacturedType, ClassIDKey> gf; 
    return gf; 
  }

  /** Register the create functions.
    @param className is the key to register the function that creates the class.
    @param fn is the function to associate with the name className.
  */
  void regCreateFn(const ClassIDKey &className, BaseCreateFn fn){
    registry[className] = fn;    
  }
};


/** A helper class to the GenericFactory.
  It is used to register several objects to the factory. You should create an instance to register the object. 
  For example:
  @code
    // Note that registerMe is the instance that register the class Base, identified by the key "Base"
    RegisterInFactory<Base, Base> registerMe("Base");
  @endcode
  @tparam AncestorType is the base class for the different ManufacturedType objects.
  @tparam ManufacturedType is the type of the object to manufactur. Note that this object should derived from AncestorType.
  @tparam ClassIDKey is the type of the key used to create the ManufacturedType class. By default it uses a std::string, but it can be any datatype.  
  @ingroup factory
  @see GenericFactory
*/
template <class AncestorType, class ManufacturedType, typename ClassIDKey=std::string>
class RegisterInFactory {
public:
  /** This function is passed to the GenericFactory to create the different instances. */
  static cv::Ptr<AncestorType> createInstance() {
    return cv::Ptr<AncestorType>(new ManufacturedType);
  }

  /** This constructor register the object to the factory.
    @param id is the key to register the ManufacturedType to the GenericFactory.    
  */
  RegisterInFactory(const ClassIDKey &id) {
    GenericFactory<AncestorType>::instance().regCreateFn(id, createInstance);
  }
};

#endif
