#pragma once

#include <optional>

#include "Client/Context.hpp"
#include "Common/Message/Fix.hpp"
#include "Common/Message/Serializer.hpp"
#include "Common/Network/UDPPackage.hpp"

namespace proc
{
    class IMessage
    {
        public:
            /// @brief Doit verifier qu'il peut géré le type de message
            ///         Puis faire des actions avec (set un status, changer une valeur) (optionnel)
            ///         puis repond un message qui va être envoié au server
            /// @param _entry message recu par le tcp
            /// @return message envoie ensuite au server
            virtual std::optional<fix::Message> process(fix::Serializer::AnonMessage &_msg, Context &_ctx) = 0;

            /// @brief Doit verifier qu'ilm peut build le type de message
            ///         Puis géré le message
            ///         Renvoie le message ou rien (std::optional)
            /// @param _tag type de message à build
            /// @return le message généré
            virtual std::optional<fix::Message> build(char _tag, Context &_ctx) const = 0;
    };

    class IUDP
    {
        public:
        /// @brief Doit verifier qu'il peut géré le type de message
            ///         Puis faire des actions avec (set un status, changer une valeur) (optionnel)
            ///         puis repond un message qui va être envoié au server
            /// @param _entry package de l'udp
            /// @return message envoie ensuite au server
            virtual std::optional<data::UDPPackage> process(const data::UDPPackage &_entry, Context &_ctx) = 0;
    };

    class IEntry
    {
        public:
            /// @brief Doit verifier qu'il peut géré le type de message
            ///         Puis faire des actions avec (set un status, changer une valeur) (optionnel)
            ///         puis repond un message qui va être envoié au server
            /// @param _entry l'entré du client
            /// @return message envoie ensuite au server
            virtual std::optional<fix::Message> process(const std::string &_entry, Context &_ctx) = 0;

            /// @brief Doit verifier qu'ilm peut build le type de message
            ///         Puis géré le message
            ///         Renvoie le message ou rien (std::optional)
            /// @param _tag type de message à build
            /// @return le message généré
            virtual std::optional<fix::Message> build(char _tag, Context &_ctx) const = 0;
    };
}