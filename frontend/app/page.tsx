"use client";

import LandingPageNavbar from "@/app/UI-Components/LandingPageNavbar";
import { Eye, Lock, Zap } from "lucide-react";
import { useEffect, useState } from "react";
import DisplayCard from "./UI-Components/DisplayCard";

export default function Component() {
  const heading = "Whhy Choose Secure Proxy?";
  const [displayText, setDisplayText] = useState("");

  useEffect(() => {
    let index = 0;
    const interval = setInterval(() => {
      setDisplayText((prevText) => prevText + heading[index]);
      index += 1;
      if (index === heading.length - 1) {
        clearInterval(interval);
      }
    }, 50);
    return () => clearInterval(interval);
  }, []);

  return (
    <main className="h-screen w-screen text-white bg-gradient-to-br from-indigo-900 via-violet-600 to-indigo-900">
      <LandingPageNavbar />
      <div className="container w-full h-full mx-auto px-4">
        <section className="min-h-screen flex items-center justify-center">
          <div className="text-center space-y-8">
            <h1 className="text-5xl font-bold mb-24">{displayText}</h1>
            <div className="grid gap-8 md:grid-cols-3">
              <DisplayCard
                image={<Lock className="w-12 h-12 text-purple-400" />}
                title={"Enhanced Privacy"}
                description="Your data is encrypted and your identity is protected"
              />
              <DisplayCard
                image={<Zap className="w-12 h-12 text-purple-400" />}
                title={"Lightning Fast"}
                description="Our servers ensure quick and efficient message delivery"
              />
              <DisplayCard
                image={<Eye className="w-12 h-12 text-purple-400" />}
                title={"No Logging Policy"}
                description="We dont keep any logs of your activities or messages"
              />
            </div>
          </div>
        </section>
      </div>
    </main>
  );
}
